/* Copyright (C) 2023 John Törnblom

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING. If not, see
<http://www.gnu.org/licenses/>.  */

// NOLINTBEGIN(*)

typedef unsigned int u_int;

void free(void*);

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cmd.h"

#define FTP_PORT 1337


/**
 * Map names of commands to function entry points.
 **/
typedef struct ftp_command {
  const char       *name;
  ftp_command_fn_t *func;
} ftp_command_t;


/**
 * Data structure used to send UI notifications on the PS5.
 **/
typedef struct notify_request
{
  int32_t type;            // 0x00
  int32_t req_id;          // 0x04
  int32_t priority;        // 0x08
  int32_t msg_id;          // 0x0C
  int32_t target_id;       // 0x10
  int32_t user_id;         // 0x14
  int32_t unk1;            // 0x18
  int32_t unk2;            // 0x1C
  int32_t app_id;          // 0x20
  int32_t error_num;       // 0x24
  int32_t unk3;            // 0x28
  char use_icon_image_uri; // 0x2C
  char message[1024];      // 0x2D
  char uri[1024];          // 0x42D
  char unkstr[1024];       // 0x82D
} notify_request_t;


/**
 * Send a UI notification request (PS5 only).
 **/
int sceKernelSendNotificationRequest(int, notify_request_t*, size_t, int);


/**
 * Global server state.
 **/
static atomic_bool g_running;
static int         g_srvfd;


/**
 * Lookup table for FTP commands.
 **/
static ftp_command_t commands[] = {
  {"CDUP", ftp_cmd_CDUP},
  {"CWD",  ftp_cmd_CWD},
  {"DELE", ftp_cmd_DELE},
  {"LIST", ftp_cmd_LIST},
  {"MKD",  ftp_cmd_MKD},
  {"NOOP", ftp_cmd_NOOP},
  {"PASV", ftp_cmd_PASV},
  {"PORT", ftp_cmd_PORT},
  {"PWD",  ftp_cmd_PWD},
  {"QUIT", ftp_cmd_QUIT},
  {"REST", ftp_cmd_REST},
  {"RETR", ftp_cmd_RETR},
  {"RMD",  ftp_cmd_RMD},
  {"RNFR", ftp_cmd_RNFR},
  {"RNTO", ftp_cmd_RNTO},
  {"SIZE", ftp_cmd_SIZE},
  {"STOR", ftp_cmd_STOR},
  {"SYST", ftp_cmd_SYST},
  {"TYPE", ftp_cmd_TYPE},
  {"USER", ftp_cmd_USER},

  // custom commands
  {"KILL", ftp_cmd_KILL},
  {"MTRW", ftp_cmd_MTRW},

  // not yet implemnted
  {"XCUP", ftp_cmd_unavailable},
  {"XMKD", ftp_cmd_unavailable},
  {"XPWD", ftp_cmd_unavailable},
  {"XRCP", ftp_cmd_unavailable},
  {"XRMD", ftp_cmd_unavailable},
  {"XRSQ", ftp_cmd_unavailable},
  {"XSEM", ftp_cmd_unavailable},
  {"XSEN", ftp_cmd_unavailable},
};


/**
 * Number of FTP commands in the lookup table.
 **/
static int nb_ftp_commands = (sizeof(commands)/sizeof(ftp_command_t));


/**
 * Read a line from a file descriptor.
 **/
static char*
ftp_readline(int fd) {
  int bufsize = 1024;
  int position = 0;
  char *buffer_backup;
  char *buffer = calloc(bufsize, sizeof(char));
  char c;

  if(!buffer) {
    perror("malloc");
    return NULL;
  }

  while(1) {
    int len = read(fd, &c, 1);
    if(len == -1 && errno == EINTR) {
      continue;
    }

    if(len <= 0) {
      free(buffer);
      return NULL;
    }

    if(c == '\r') {
      buffer[position] = '\0';
      position = 0;
      continue;
    }

    if(c == '\n') {
      return buffer;
    }

    buffer[position++] = c;

    if(position >= bufsize) {
      bufsize += 1024;
      buffer_backup = buffer;
      buffer = realloc(buffer, bufsize);
      if(!buffer) {
	perror("realloc");
	free(buffer_backup);
	return NULL;
      }
    }
  }
}


/**
 * Execute an FTP command.
 **/
static int
ftp_execute(ftp_env_t *env, char *line) {
  char *sep = strchr(line, ' ');
  char *arg = strchr(line, 0);

  if(sep) {
    sep[0] = 0;
    arg = sep + 1;
  }

  for(int i=0; i<nb_ftp_commands; i++) {
    if(strcmp(line, commands[i].name)) {
      continue;
    }

    return commands[i].func(env, arg);
  }

  return ftp_cmd_unknown(env, arg);
}


/**
 * Greet a new FTP connection.
 **/
static int
ftp_greet(ftp_env_t *env) {
  const char *msg = "220 Service is ready\r\n";
  size_t len = strlen(msg);

  if(write(env->active_fd, msg, len) != len) {
    return -1;
  }

  return 0;
}


/**
 * Entry point for new FTP connections.
 **/
static void*
ftp_thread(void *args) {
  ftp_env_t env;
  bool running;
  char *line;

  env.data_fd     = -1;
  env.passive_fd  = -1;
  env.active_fd   = (int)(long)args;
  env.srv_running = &g_running;

  env.type        = 'A';
  env.data_offset = 0;

  strcpy(env.cwd, "/");
  __builtin_memset(env.rename_path, 0, sizeof(env.rename_path));
  __builtin_memset(&env.data_addr, 0, sizeof(env.data_addr));

  running = !ftp_greet(&env);

  while(running && atomic_load(&g_running)) {
    if(!(line=ftp_readline(env.active_fd))) {
      break;
    }

    if(ftp_execute(&env, line)) {
      running = false;
    }

    free(line);
  }

  if(env.active_fd > 0) {
    close(env.active_fd);
  }

  if(env.passive_fd > 0) {
    close(env.passive_fd);
  }

  if(!atomic_load(&g_running)) {
    shutdown(g_srvfd, SHUT_RDWR);
  }

  if(env.data_fd > 0) {
    close(env.data_fd);
  }

  pthread_exit(NULL);

  return NULL;
}


/**
 * Serve FTP on a given port.
 **/
static int
ftp_serve(uint16_t port) {
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  char ip[INET_ADDRSTRLEN];
  struct ifaddrs *ifaddr;
  notify_request_t req;
  socklen_t addr_len;
  pthread_t trd;
  int connfd;

  if(getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    return EXIT_FAILURE;
  }

  signal(SIGPIPE, SIG_IGN);

  // Enumerate all AF_INET IPs
  for(struct ifaddrs *ifa=ifaddr; ifa!=NULL; ifa=ifa->ifa_next) {
    if(ifa->ifa_addr == NULL) {
      continue;
    }

    if(ifa->ifa_addr->sa_family != AF_INET) {
      continue;
    }

    struct sockaddr_in *in = (struct sockaddr_in*)ifa->ifa_addr;
    inet_ntop(AF_INET, &(in->sin_addr), ip, sizeof(ip));

    bzero(&req, sizeof(req));
    snprintf(req.message, sizeof(req.message), "Serving FTP on %s:%d (%s)",
	    ip, port, ifa->ifa_name);

    sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
    puts(req.message);
  }

  freeifaddrs(ifaddr);

  if((g_srvfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return EXIT_FAILURE;
  }

  if(setsockopt(g_srvfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
    perror("setsockopt");
    return EXIT_FAILURE;
  }

  __builtin_memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  if(bind(g_srvfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
    perror("bind");
    return EXIT_FAILURE;
  }

  if(listen(g_srvfd, 5) != 0) {
    perror("listen");
    return EXIT_FAILURE;
  }

  addr_len = sizeof(client_addr);
  atomic_init(&g_running, true);

  while(atomic_load(&g_running)) {
    if((connfd=accept(g_srvfd, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
      if(errno == 0xa3) {
		    close(g_srvfd);
        bzero(&req, sizeof(req));
        snprintf(req.message, sizeof(req.message), "Server closing due to system suspension\n");
        sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
        return EXIT_SUCCESS;
	  }
      perror("accept");
      continue;
    }

    pthread_create(&trd, NULL, ftp_thread, (void*)(long)connfd);
  }

  close(g_srvfd);
  bzero(&req, sizeof(req));
  snprintf(req.message, sizeof(req.message), "Server killed");
  sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
  puts(req.message);
  
  return EXIT_SUCCESS;
}


extern void sce_remount();


/**
 * Launch payload.
 **/
int start_ftp() {
	sce_remount();
    return ftp_serve(FTP_PORT);
}


// NOLINTEND(*)
