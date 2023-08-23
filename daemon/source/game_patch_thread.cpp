#include <pthread.h>

#include "elf/elf.hpp"
#include "dbg/dbg.hpp"
#include "hijacker/hijacker.hpp"

#include "game_patch_thread.hpp"
#include "game_patch_memory.hpp"

#include "game_patch.hpp"
#include "notify.hpp"
#include "print.hpp"

#include "pad.hpp"

extern "C"
{
#include "../extern/pfd_sfo_tools/sfopatcher/src/sfo.h"
#include "../extern/tiny-json/tiny-json.h"
	// pause resume
	int32_t sceKernelPrepareToSuspendProcess(pid_t pid);
	int32_t sceKernelSuspendProcess(pid_t pid);
	int32_t sceKernelPrepareToResumeProcess(pid_t pid);
	int32_t sceKernelResumeProcess(pid_t pid);
}

bool g_game_patch_thread_running = false;

static void SuspendApp(pid_t pid)
{
	print_ret(sceKernelPrepareToSuspendProcess(pid));
	print_ret(sceKernelSuspendProcess(pid));
}

static void ResumeApp(pid_t pid)
{
	usleep(1000);
	print_ret(sceKernelPrepareToResumeProcess(pid));
	print_ret(sceKernelResumeProcess(pid));
}

enum AppType : uint32_t
{
	PS4_APP = 0,
	PS5_APP = 1,
};

#define _MAX_PATH 260

static int32_t get_app_info(const char *title_id, char *out_app_ver, char *out_master_ver, char *out_app_content_id, const AppType app_mode)
{
	int32_t read_ret = 0;
	switch (app_mode)
	{
	case PS4_APP: // CUSA Apps
	{
		char sfo_path[_MAX_PATH] = {0};
		(void)snprintf(sfo_path, sizeof(sfo_path), "/system_data/priv/appmeta/%s/param.sfo", title_id);
		sfo_context_t *sfo = sfo_alloc();
		int32_t sfo_ret = sfo_read(sfo, sfo_path);
		if (sfo_ret == -1)
		{
			_printf("Failed to get app information from param.sfo!\nsfo_path: %s\n", sfo_path);
		}
		else
		{
			const char *app_app_ver = (const char *)sfo_get_param_value(sfo, "APP_VER");
			const char *app_master_ver = (const char *)sfo_get_param_value(sfo, "VERSION");
			const char *app_content_id = (const char *)sfo_get_param_value(sfo, "CONTENT_ID");
			if (app_app_ver && app_master_ver && app_content_id)
			{
				_printf("\n"
						"sfo_read: 0x%08x\n"
						"APP_VER: %s\n"
						"VERSION: %s\n"
						"CONTENT_ID: %s\n",
						read_ret,
						app_app_ver,
						app_master_ver,
						app_content_id);
				strncpy(out_app_ver, app_app_ver, 8);
				strncpy(out_master_ver, app_master_ver, 8);
				strncpy(out_app_content_id, app_content_id, 40);
				read_ret = 0;
			}
			else
			{
				read_ret = -1;
			}
		}
		if (sfo)
		{
			sfo_free(sfo);
		}
		break;
	}
	case PS5_APP: // PPSA Apps
	{
		char sfo_path[_MAX_PATH] = {0};
		(void)snprintf(sfo_path, sizeof(sfo_path), "/system_data/priv/appmeta/%s/param.json", title_id);
		FILE *file = fopen(sfo_path, "r");
		if (!file)
		{
			_printf("Failed to get app information from param.sfo!\nsfo_path: %s\n", sfo_path);
			read_ret = -1;
			break;
		}

		fseek(file, 0, SEEK_END);
		size_t file_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		char *json_data = (char *)malloc(file_size + 1);
		if (!json_data)
		{
			fclose(file);
			read_ret = -1;
			break;
		}

		fread(json_data, 1, file_size, file);
		fclose(file);
#define TOKENS 256
		json_t pool[TOKENS];
		json_t const *my_json = json_create(json_data, pool, TOKENS);
#undef TOKENS
		if (!my_json)
		{
			_printf("Error json create %s\n", sfo_path);
			read_ret = -1;
			break;
		}

		const char *contentId = json_getPropertyValue(my_json, "contentId");
		const char *contentVersion = json_getPropertyValue(my_json, "contentVersion");
		const char *masterVersion = json_getPropertyValue(my_json, "masterVersion");

		if (contentId && contentVersion && masterVersion)
		{
			_printf("\n"
					"json_getPropertyValue:\n"
					"contentId: %s (0x%p)\n"
					"contentVersion: %s (0x%p)\n"
					"masterVersion: %s (0x%p)\n",
					contentId, &contentId,
					contentVersion, &contentVersion,
					masterVersion, &masterVersion);
			strncpy(out_app_ver, contentVersion, 16);
			strncpy(out_master_ver, masterVersion, 16);
			strncpy(out_app_content_id, contentId, 40);
			read_ret = 0;
		}
		else
		{
			_printf("Failed to retrieve %s values.\nContents of file:\n", sfo_path);
			puts(json_data);
			read_ret = -1;
		}
		if (json_data)
		{
			free(json_data);
		}
		break;
	}
	}
	return read_ret;
}

// we only use this macro here
// don't keep it outside this scope
// we must use `__builtin_strlen` here for constexpr
// libcinternal strlen does not compile time compute the string length due to being in shared lib?
#define startsWith(str1, str2) (strncmp(str1, str2, __builtin_strlen(str2)) == 0)

bool isAlive(pid_t v) {
	for (pid_t pid : dbg::getAllPids()) {
		if (pid == v) {
			// _printf("(pid == v): %d\n", pid);
			return true;
		}
	}
	return false;
}

extern "C" {
	int32_t sceUserServiceInitialize(int32_t *priority);
	int32_t sceUserServiceGetForegroundUser(int32_t *new_id);
}

bool checkPatchButton(OrbisPadData *pData)
{
	return (pData->buttons & ORBIS_PAD_BUTTON_SQUARE) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_TRIANGLE);
}

bool checkKillButton(OrbisPadData *pData)
{
	return (pData->buttons & ORBIS_PAD_BUTTON_L3) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_R3) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_L1) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_R1) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_SQUARE);
}

void *GamePatch_Thread(void *unused)
{
	printf_notification("Game Patch thread running.\nBuilt: " __DATE__ " @ " __TIME__);
	int32_t user_id = 0;
	int32_t pad_handle = 0;
	int32_t priority = 256;
	print_ret(sceUserServiceInitialize(&priority));
	print_ret(sceUserServiceGetForegroundUser(&user_id));
	_printf("priority: 0x%08x\n", priority);
	_printf("user_id: 0x%08x\n", user_id);
	if (user_id > 0)
	{
		print_ret(scePadInit());
		pad_handle = scePadOpen(user_id, 0, 0, nullptr);
		_printf("scePadOpen: 0x%08x\n", pad_handle);
		print_ret(scePadSetProcessPrivilege(1));
	}
	else
	{
		printf_notification("Failed to obtain current user id! Pad functions will not work.");
		user_id = 0;
	}
	bool found_app = false;
	// multiple self games
	bool fast_sleep_timer = false;
	pid_t target_running_pid = 0;
	g_game_patch_thread_running = true;

	bool prevTogglePressed = false;
	bool doPatchGames = true;

	while (g_game_patch_thread_running)
	{
		OrbisPadData pData{};
		if (pad_handle && !found_app)
		{
			int32_t ret = scePadReadState(pad_handle, &pData);
			if (ret == 0 && pad_handle > 0 && pData.connected)
			{
				bool currentTogglePressed = checkPatchButton(&pData);
				if (currentTogglePressed && !prevTogglePressed)
				{
					doPatchGames = !doPatchGames;
					printf_notification("User requested to patch games: %s", doPatchGames ? "true" : "false");
					_printf("doPatchGames: 0x%02x\n", doPatchGames);
				}
				prevTogglePressed = currentTogglePressed;
				if (checkKillButton(&pData))
				{
					//printf_notification("User requested exit for Game Patch thread.");
					g_game_patch_thread_running = false;
					continue;
				}
			}
		}
		if (!doPatchGames)
		{
			continue;
		}
		if (dbg::getProcesses().length() == 0)
		{
			// g_game_patch_thread_running = false;
			_puts("(dbg::getProcesses().length() == 0), continuing");
			continue;
		}
		for (auto p : dbg::getProcesses())
		{
			const pid_t app_pid = p.pid();
			const UniquePtr<Hijacker> executable = Hijacker::getHijacker(app_pid);
			uintptr_t text_base = 0;
			uint64_t text_size = 0;
			if (executable)
			{
				text_base = executable->imagebase();
				text_size = executable->getEboot()->getTextSection()->sectionLength();
			}
			else
			{
				continue;
			}
			if (text_base == 0 || text_size == 0)
			{
				continue;
			}
			const auto app = getProc(app_pid);
			if ((startsWith(app->titleId().c_str(), "CUSA")) && text_base && !found_app)
			{
				char app_ver[8] = {0};
				char master_ver[8] = {0};
				char content_id[40] = {0};
				int32_t ret = get_app_info(app->titleId().c_str(), app_ver, master_ver, content_id, PS4_APP);
				if (ret != 0)
				{
					// something went wrong
					printf_notification("get_app_info(%s) failed! %d", app->titleId().c_str(), ret);
					continue;
				}
				else if (ret == 0)
				{
					found_app = true;
					target_running_pid = app_pid;
				}
				if (startsWith(p.name().c_str(), "eboot.bin"))
				{
					if ((startsWith(app->titleId().c_str(), "CUSA00547") ||
						 startsWith(app->titleId().c_str(), "CUSA03694") ||
						 startsWith(app->titleId().c_str(), "CUSA04934") ||
						 startsWith(app->titleId().c_str(), "CUSA04943")) &&
						(startsWith(app_ver, "01.11")))
					{
						// we need to sleep the thread after suspension
						// because this will cause a kernel panic when user quits the process after sometime
						// if we suspend and resume work immediately in this current state
						// the kernel will not be very happy with us.
						SuspendApp(app_pid);
						DoPatch_GravityDaze2_111(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA00900") ||
							  startsWith(app->titleId().c_str(), "CUSA00207") ||
							  startsWith(app->titleId().c_str(), "CUSA03173") ||
							  startsWith(app->titleId().c_str(), "CUSA00208") ||
							  startsWith(app->titleId().c_str(), "CUSA01363")) &&
							 (startsWith(app_ver, "01.09")))
					{
						SuspendApp(app_pid);
						DoPatch_Bloodborne109(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA03041") ||
							  startsWith(app->titleId().c_str(), "CUSA08519") ||
							  startsWith(app->titleId().c_str(), "CUSA08568")) &&
							 (startsWith(app_ver, "01.29")))
					{
						SuspendApp(app_pid);
						DoPatch_RDR2_129(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA00035") ||
							  startsWith(app->titleId().c_str(), "CUSA00070") ||
							  startsWith(app->titleId().c_str(), "CUSA00076") ||
							  startsWith(app->titleId().c_str(), "CUSA00100") ||
							  startsWith(app->titleId().c_str(), "CUSA00670") ||
							  startsWith(app->titleId().c_str(), "CUSA00670")) &&
							 (startsWith(app_ver, "01.02")))
					{
						SuspendApp(app_pid);
						DoPatch_TheOrder1886_102(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA00003") ||
							  startsWith(app->titleId().c_str(), "CUSA00064") ||
							  startsWith(app->titleId().c_str(), "CUSA00066") ||
							  startsWith(app->titleId().c_str(), "CUSA00093") ||
							  startsWith(app->titleId().c_str(), "CUSA00879")) &&
							 (startsWith(app_ver, "01.28")))
					{
						SuspendApp(app_pid);
						DoPatch_DriveClub_128(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA03627") ||
							  startsWith(app->titleId().c_str(), "CUSA03745") ||
							  startsWith(app->titleId().c_str(), "CUSA04936")) &&
							 (startsWith(app_ver, "01.03")))
					{
						SuspendApp(app_pid);
						DoPatch_TheLastGuardian_103(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA13795")) &&
							 (startsWith(app_ver, "01.21")))
					{
						SuspendApp(app_pid);
						DoPatch_CTR_121(app_pid, text_base, 1);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA14876")) &&
							 (startsWith(app_ver, "01.21")))
					{
						SuspendApp(app_pid);
						DoPatch_CTR_121(app_pid, text_base, 2);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA08034")) &&
							 (startsWith(app_ver, "01.00") ||
							  startsWith(app_ver, "01.01")))
					{
						SuspendApp(app_pid);
						DoPatch_SOTC_100(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): Debug Menu Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA01399") ||
							  startsWith(app->titleId().c_str(), "CUSA02320") ||
							  startsWith(app->titleId().c_str(), "CUSA02343") ||
							  startsWith(app->titleId().c_str(), "CUSA02344") ||
							  startsWith(app->titleId().c_str(), "CUSA02826")) &&
							 startsWith(app_ver, "01.00"))
					{
						SuspendApp(app_pid);
						DoPatch_BigCollection_100(app_pid, text_base, 1);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = true;
						printf_notification("%s (%s): Debug Menu Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA07820") ||
							  startsWith(app->titleId().c_str(), "CUSA10249") ||
							  startsWith(app->titleId().c_str(), "CUSA13986") ||
							  startsWith(app->titleId().c_str(), "CUSA14006")) &&
							 (startsWith(app_ver, "01.00")))
					{
						SuspendApp(app_pid);
						DoPatch_t2ps4(app_pid, text_base, 0x100);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS + Debug Menu Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA07820") ||
							  startsWith(app->titleId().c_str(), "CUSA10249") ||
							  startsWith(app->titleId().c_str(), "CUSA13986") ||
							  startsWith(app->titleId().c_str(), "CUSA14006")) &&
							 (startsWith(app_ver, "01.09")))
					{
						SuspendApp(app_pid);
						DoPatch_t2ps4(app_pid, text_base, 0x109);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS + Debug Menu Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA00552") ||
							  startsWith(app->titleId().c_str(), "CUSA00556") ||
							  startsWith(app->titleId().c_str(), "CUSA00557") ||
							  startsWith(app->titleId().c_str(), "CUSA00559")) &&
							 (startsWith(app_ver, "01.11")))
					{
						SuspendApp(app_pid);
						DoPatch_t1ps4_111(app_pid, text_base);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS + Debug Menu Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA01127") ||
							  startsWith(app->titleId().c_str(), "CUSA01114") ||
							  startsWith(app->titleId().c_str(), "CUSA01098")) &&
							 (startsWith(app_ver, "01.00")))
					{
						SuspendApp(app_pid);
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x00d8a713), "be00000000");
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA09254") ||
							  startsWith(app->titleId().c_str(), "CUSA09264") ||
							  startsWith(app->titleId().c_str(), "CUSA12635") ||
							  startsWith(app->titleId().c_str(), "CUSA13217") ||
							  startsWith(app->titleId().c_str(), "CUSA13318")) &&
							 (startsWith(app_ver, "01.32")))
					{
						SuspendApp(app_pid);
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x0264c85d), "41be00000000");
						write_bytes(app_pid, NO_ASLR(0x005b6bcd), "41be01000000");
						write_bytes(app_pid, NO_ASLR(0x005b6bd3), "eb1a");
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA00133") ||
							  startsWith(app->titleId().c_str(), "CUSA00135") ||
							  startsWith(app->titleId().c_str(), "CUSA01009")) &&
							 (startsWith(app_ver, "01.15")))
					{
						SuspendApp(app_pid);
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x009fa57e), "be00000000");
						write_bytes(app_pid, NO_ASLR(0x009fb766), "41bc04000000");
						write_bytes(app_pid, NO_ASLR(0x009fb9e1), "0f85a9000000");
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "CUSA07399") ||
							  startsWith(app->titleId().c_str(), "CUSA07402")) &&
							 (startsWith(app_ver, "01.07")))
					{
						SuspendApp(app_pid);
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x00f52de0), "be00000000909090909090909090");
						// Resolution Patch
						// 2560x1440 -> 3200x1800
						// Doesn't seem to be a difference in term of drops when new area loads
						write_bytes32(app_pid, NO_ASLR(0x00529cdb), 0x3fd55555); // 1.667f
						write_bytes32(app_pid, NO_ASLR(0x01c75680), 0x3fd55555); // 1.667f
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
				}

				// multiple selfs
				// sometimes work sometimes doesn't
				// this thread wait model is stinky
				// big2
				if (startsWith(p.name().c_str(), "big2-ps4_Shipping.elf") && text_base && !found_app)
				{
					if (startsWith(app->titleId().c_str(), "CUSA02320"))
					{
						SuspendApp(app_pid);
						DoPatch_BigCollection_100(app_pid, text_base, 2);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = true;
						printf_notification("%s (%s): Debug Menu Patched!", app->titleId().c_str(), p.name().c_str());
						ResumeApp(app_pid);
					}
				}
				// big3
				else if (startsWith(p.name().c_str(), "big3-ps4_Shipping.elf") && text_base && !found_app)
				{
					if (startsWith(app->titleId().c_str(), "CUSA02320"))
					{
						SuspendApp(app_pid);
						DoPatch_BigCollection_100(app_pid, text_base, 3);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = true;
						printf_notification("%s (%s): Debug Menu Patched!", app->titleId().c_str(), p.name().c_str());
						ResumeApp(app_pid);
					}
				}
			}
			else if ((startsWith(app->titleId().c_str(), "PPSA")) && text_base && !found_app)
			{
				char app_ver[16] = {0};	   // `contentVersion`
				char master_ver[16] = {0}; // `masterVersion`
				char content_id[40] = {0}; // `contentId`
				int32_t ret = get_app_info(app->titleId().c_str(), app_ver, master_ver, content_id, PS5_APP);
				if (ret != 0)
				{
					// something went wrong
					printf_notification("get_app_info(%s) failed! %d", app->titleId().c_str(), ret);
					continue;
				}
				else if (ret == 0)
				{
					found_app = true;
					target_running_pid = app_pid;
				}
				// eboot.bin games
				if (startsWith(p.name().c_str(), "eboot.bin"))
				{
					if ((startsWith(app->titleId().c_str(), "PPSA05684") ||
						 startsWith(app->titleId().c_str(), "PPSA05389") ||
						 startsWith(app->titleId().c_str(), "PPSA05686") ||
						 startsWith(app->titleId().c_str(), "PPSA05685")) &&
						(startsWith(app_ver, "01.000.000")))
					{
						SuspendApp(app_pid);
						DoPatch_Big4R_100(app_pid, text_base, 1);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = true;
						printf_notification("%s (%s): Debug Menu Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
					else if ((startsWith(app->titleId().c_str(), "PPSA01341") ||
							  startsWith(app->titleId().c_str(), "PPSA01342")) &&
							 (startsWith(app_ver, "01.000.000")))
					{
						SuspendApp(app_pid);
						DoPatch_DemonSouls(app_pid, text_base, 0x100);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = false;
						printf_notification("%s (%s): 60 FPS Cinematic Mode Patched!", app->titleId().c_str(), app_ver);
						ResumeApp(app_pid);
					}
				}
				// tll big4r
				else if (startsWith(p.name().c_str(), "tllr-boot.bin") && text_base && !found_app)
				{
					if ((startsWith(app->titleId().c_str(), "PPSA05684") ||
						 startsWith(app->titleId().c_str(), "PPSA05389") ||
						 startsWith(app->titleId().c_str(), "PPSA05686") ||
						 startsWith(app->titleId().c_str(), "PPSA05685")) &&
						(startsWith(app_ver, "01.000.000")))
					{
						SuspendApp(app_pid);
						DoPatch_Big4R_100(app_pid, text_base, 2);
						target_running_pid = app_pid;
						found_app = true;
						fast_sleep_timer = true;
						printf_notification("%s (%s): Debug Menu Patched!", app->titleId().c_str(), p.name().c_str());
						ResumeApp(app_pid);
					}
				}
			}
		}
		if (found_app)
		{
			if (fast_sleep_timer)
			{
				usleep(500);
			}
			else
			{
				sleep(1);
			}
			found_app = true;
		}
		else
		{
			usleep(500);
			found_app = false;
		}
		if (!isAlive(target_running_pid))
		{
			target_running_pid = 0;
			found_app = false;
		}
		else
		{
			found_app = true;
		}
	}

	if (pad_handle)
	{
		print_ret(scePadClose(pad_handle));
	}
	printf_notification("Game Patch thread has requested to stop");
	pthread_exit(nullptr);
	return nullptr;
}

#undef _MAX_PATH
#undef startsWith
