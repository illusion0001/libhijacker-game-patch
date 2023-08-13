// NOLINTBEGIN(*)

typedef int (*_nanosleep_t)(unsigned int);
typedef int (*execve_t)(const char *path, const char **argv, const char **envp);

// insert a "MOV execve, RCX" at the beginning of the hook shellcode
// insert a "MOV _nanosleep, R8" at the beginning of the hook shellcode
/*
48 b9 xx xx xx xx xx xx xx xx
49 b8 xx xx xx xx xx xx xx xx
*/

int execve_hook(const char *path, const char **argv, const char **envp, execve_t execve, _nanosleep_t _nanosleep) {
	// All we're doing here is delaying just long enough for another process to come
	// in and use PT_ATTACH. If we are starting a normal app then execve will continue
	// as normal. Otherwise after waitpid, ptrace(PT_CONTINUE, pid, (caddr_t)1, 0), waitpid
	// it will then be stopped after the completion of execve.
	// NOTE: FS is 0 at this point in time and what this process can do until the execve
	// call has completed is very limited.
	_nanosleep(2000000000);
	return execve(path, argv, envp);
}

// NOLINTEND(*)
