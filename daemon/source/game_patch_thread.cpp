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
#include "titleid_map.hpp"

#include "game_patch_xml.hpp"

extern "C"
{
#include "../extern/pfd_sfo_tools/sfopatcher/src/sfo.h"
#include "../extern/tiny-json/tiny-json.h"
	int32_t sceKernelPrepareToSuspendProcess(pid_t pid);
	int32_t sceKernelSuspendProcess(pid_t pid);
	int32_t sceKernelPrepareToResumeProcess(pid_t pid);
	int32_t sceKernelResumeProcess(pid_t pid);
	int32_t sceUserServiceInitialize(int32_t *priority);
	int32_t sceUserServiceGetForegroundUser(int32_t *new_id);
	int32_t sceSysmoduleLoadModuleInternal(uint32_t moduleId);
	int32_t sceSysmoduleUnloadModuleInternal(uint32_t moduleId);
	int32_t sceVideoOutOpen();
	int32_t sceVideoOutConfigureOutput();
	int32_t sceVideoOutIsOutputSupported();
}

int32_t g_game_patch_thread_running = false;

enum AppType : uint32_t
{
	PS4_APP = 1 << 4,
	PS5_APP = 1 << 5,
};

#define _MAX_PATH 260
#define APP_VER_SIZE 16
#define MASTER_VER_SIZE 16
#define CONTENT_ID_SIZE 64

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
				strncpy(out_app_ver, app_app_ver, APP_VER_SIZE);
				strncpy(out_master_ver, app_master_ver, MASTER_VER_SIZE);
				strncpy(out_app_content_id, app_content_id, CONTENT_ID_SIZE);
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
		constexpr uint32_t MAX_TOKENS = 256;
		json_t pool[MAX_TOKENS]{};
		json_t const *my_json = json_create(json_data, pool, MAX_TOKENS);
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
			strncpy(out_app_ver, contentVersion, APP_VER_SIZE);
			strncpy(out_master_ver, masterVersion, MASTER_VER_SIZE);
			strncpy(out_app_content_id, contentId, CONTENT_ID_SIZE);
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

#undef _MAX_PATH

const bool isAlive(const pid_t pid)
{
	if (pid <= 0)
	{
		return false;
	}
	for (const pid_t v : dbg::getAllPids())
	{
		if (pid == v)
		{
			// _printf("(pid == v): %d\n", pid);
			return true;
		}
	}
	return false;
}

const int32_t checkPatchButton(const OrbisPadData *pData)
{
	return (pData->buttons & ORBIS_PAD_BUTTON_SQUARE) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_TRIANGLE);
}

const int32_t checkFlipRateButton(const OrbisPadData *pData)
{
	return (pData->buttons & ORBIS_PAD_BUTTON_SQUARE) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_CIRCLE);
}

const int32_t checkKillButton(const OrbisPadData *pData)
{
	return (pData->buttons & ORBIS_PAD_BUTTON_L3) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_R3) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_L1) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_R1) &&
		   (pData->buttons & ORBIS_PAD_BUTTON_SQUARE);
}

const int32_t check120HzButton(const OrbisPadData *pData)
{
	return (pData->buttons & ORBIS_PAD_BUTTON_CROSS) || (pData->buttons & ORBIS_PAD_BUTTON_CIRCLE);
}

int32_t g_foundApp = false;
int32_t g_doPatchGames = false;
int32_t g_UniversalFlipRatePatch = false;

void *GamePatch_InputThread(void *unused)
{
	(void)unused;
	printf_notification("Game Patch Input Thread Started.");
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
	int32_t prevTogglePressed = false;
	int32_t prevTogglePressed2 = false;
	while (g_game_patch_thread_running)
	{
		OrbisPadData pData{};
		if (pad_handle && !g_foundApp)
		{
			int32_t ret = scePadReadState(pad_handle, &pData);
			if (ret == 0 && pad_handle > 0 && pData.connected)
			{
				int32_t currentTogglePressed{};
				int32_t currentTogglePressed2{};
				currentTogglePressed = checkPatchButton(&pData);
				if (currentTogglePressed && !prevTogglePressed)
				{
					g_doPatchGames = !g_doPatchGames;
					printf_notification("User requested to patch games: %s", g_doPatchGames ? "true" : "false");
				}
				prevTogglePressed = currentTogglePressed;
				currentTogglePressed2 = checkFlipRateButton(&pData);
				if (currentTogglePressed2 && !prevTogglePressed2)
				{
					g_UniversalFlipRatePatch = !g_UniversalFlipRatePatch;
					printf_notification("User requested to always patch fliprate to 0: %s", g_UniversalFlipRatePatch ? "true" : "false");
				}
				prevTogglePressed2 = currentTogglePressed2;
				if (checkKillButton(&pData))
				{
					g_game_patch_thread_running = false;
					continue;
				}
				g_isPatch120Hz = false;
				if (check120HzButton(&pData))
				{
					g_isPatch120Hz = true;
				}
			}
		}
		usleep(1000);
	}
	if (pad_handle)
	{
		print_ret(scePadClose(pad_handle));
	}
	printf_notification("Game Patch Input thread has requested to stop");
	pthread_exit(nullptr);
	return nullptr;
}

static void SuspendApp(pid_t pid)
{
	sceKernelPrepareToSuspendProcess(pid);
	sceKernelSuspendProcess(pid);
}

static void ResumeApp(pid_t pid)
{
	// we need to sleep the thread after suspension
	// because this will cause a kernel panic when user quits the process after sometime
	// the kernel will not be very happy with us.
	usleep(1000);
	sceKernelPrepareToResumeProcess(pid);
	sceKernelResumeProcess(pid);
}

int32_t patch_SetFlipRate(const Hijacker &hijacker, const pid_t pid)
{
	static constexpr Nid sceVideoOutSetFlipRate_Nid{"CBiu4mCE1DA"};
	UniquePtr<SharedLib> lib = hijacker.getLib("libSceVideoOut.sprx"_sv);
	while (lib == nullptr)
	{
		lib = hijacker.getLib("libSceVideoOut.sprx"_sv);
		// usleep(100);
	}
	SuspendApp(pid);
	if (lib)
	{
		uint8_t is_mov_r14d_esi[3]{};
		const auto sceVideoOutSetFlipRate_ = hijacker.getFunctionAddress(lib.get(), sceVideoOutSetFlipRate_Nid);
		dbg::read(pid, sceVideoOutSetFlipRate_ + 0xa, &is_mov_r14d_esi, sizeof(is_mov_r14d_esi));
		if (is_mov_r14d_esi[0] == 0x41 && is_mov_r14d_esi[1] == 0x89 && is_mov_r14d_esi[2] == 0xf6)
		{
			uint8_t xor_r14d_r14d[3] = {0x45, 0x31, 0xf6};
			dbg::write(pid, sceVideoOutSetFlipRate_ + 0xa, xor_r14d_r14d, sizeof(xor_r14d_r14d));
			printf_notification("sceVideoOutSetFlipRate Patched");
		}
		else
		{
			// in case user loaded modified prx, lets make it do nothing
			printf_notification("Cannot find sceVideoOutSetFlipRate location\nPatching it to return 0.");
			uint8_t xor_eax_eax_ret[3] = {0x31, 0xc0, 0xc3};
			dbg::write(pid, sceVideoOutSetFlipRate_, xor_eax_eax_ret, sizeof(xor_eax_eax_ret));
		}
	}
	ResumeApp(pid);
	return 0;
}

void *GamePatch_Thread(void *unused)
{
	(void)unused;
	printf_notification("Game Patch thread running.\nBuilt: " __DATE__ " @ " __TIME__);
	makeDefaultXml_Cfg();
	makeDefaultXml_List();
	int32_t is120HzUsable = false;
	FlipRate_ConfigureOutput_Ptr = 0;
	FlipRate_isVideoModeSupported_Ptr = 0;
	int32_t module_load = 0;
	constexpr uint32_t ORBIS_SYSMODULE_INTERNAL_VIDEO_OUT = 0x80000022;
	module_load = sceSysmoduleLoadModuleInternal(ORBIS_SYSMODULE_INTERNAL_VIDEO_OUT);
	_printf("sceSysmoduleLoadModuleInternal: 0x%08x\n", module_load);
	if (uintptr_t(sceVideoOutOpen) && uintptr_t(sceVideoOutIsOutputSupported) && uintptr_t(sceVideoOutConfigureOutput))
	{
		FlipRate_ConfigureOutput_Ptr = uintptr_t(sceVideoOutConfigureOutput) - uintptr_t(sceVideoOutOpen);
		FlipRate_isVideoModeSupported_Ptr = uintptr_t(sceVideoOutIsOutputSupported) - uintptr_t(sceVideoOutOpen);
		_printf("sceVideoOutSetFlipRate: 0x%p\n", sceVideoOutOpen);
		_printf("sceVideoOutConfigureOutput: 0x%p\n", sceVideoOutConfigureOutput);
		_printf("sceVideoOutIsOutputSupported: 0x%p\n", sceVideoOutIsOutputSupported);
		_printf("FlipRate_ConfigureOutput_Ptr: 0x%lx\n", FlipRate_ConfigureOutput_Ptr);
		_printf("FlipRate_isVideoModeSupported_Ptr: 0x%lx\n", FlipRate_isVideoModeSupported_Ptr);
	}
	module_load = sceSysmoduleUnloadModuleInternal(ORBIS_SYSMODULE_INTERNAL_VIDEO_OUT);
	_printf("sceSysmoduleUnloadModuleInternal: 0x%08x\n", module_load);
	if (FlipRate_ConfigureOutput_Ptr > 0 && FlipRate_isVideoModeSupported_Ptr > 0)
	{
		is120HzUsable = true;
	}
	else
	{
		is120HzUsable = false;
	}

	pid_t target_running_pid = -1;
	g_game_patch_thread_running = true;
	pid_t shellcore_pid = 0;

	int32_t doPatchGames = true;
	{
		const UniquePtr<Hijacker> executable = Hijacker::getHijacker("SceShellCore"_sv);
		uintptr_t text_base = 0;
		uint64_t text_size = 0;
		if (executable)
		{
			text_base = executable->getEboot()->getTextSection()->start();
			text_size = executable->getEboot()->getTextSection()->sectionLength();
			shellcore_pid = executable->getPid();
			if (is120HzUsable)
			{
				if (patchShellCore(shellcore_pid, text_base, text_size))
				{
					printf_notification("Patches for ShellCore has been installed.");
				}
				else
				{
					printf_notification("Failed to install patches for ShellCore.");
				}
			}
		}
		else
		{
			printf_notification("SceShellCore not found");
		}
	}

	while (g_game_patch_thread_running)
	{
		if (!doPatchGames)
		{
			usleep(1000);
			continue;
		}
		if (dbg::getProcesses().length() == 0)
		{
			printf_notification("(dbg::getProcesses().length() == 0), continuing");
			continue;
		}
		for (auto p : dbg::getProcesses())
		{
			if (g_foundApp)
			{
				usleep(1000);
				break;
			}
			const pid_t app_pid = p.pid();
			const UniquePtr<Hijacker> executable = Hijacker::getHijacker(app_pid);
			uintptr_t text_base = 0;
			uint64_t text_size = 0;
			if (executable)
			{
				text_base = executable->getEboot()->getTextSection()->start();
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
			String titleId = app->titleId();
			StringView process_name = p.name();
			const char *app_id = titleId.c_str();
			const char *process_name_c_str = process_name.c_str();
			if (text_base && !g_foundApp &&
				(startsWith(app_id, "CUSA") ||
				 startsWith(app_id, "PCAS") ||
				 startsWith(app_id, "PCJS")))
			{
				char app_ver[APP_VER_SIZE]{};
				char master_ver[MASTER_VER_SIZE]{};
				char content_id[CONTENT_ID_SIZE]{};
				int32_t ret = get_app_info(app_id, app_ver, master_ver, content_id, PS4_APP);
				if (ret != 0)
				{
					// something went wrong
					printf_notification("get_app_info(%s) failed! %d", app_id, ret);
					continue;
				}
				else if (ret == 0)
				{
					g_foundApp = true;
					target_running_pid = app_pid;
				}
				SuspendApp(app_pid);
				if (startsWith(process_name_c_str, "eboot.bin"))
				{
					if ((startsWith(app_id, "CUSA00547") ||
						 startsWith(app_id, "CUSA03694") ||
						 startsWith(app_id, "CUSA04934") ||
						 startsWith(app_id, "CUSA04943")) &&
						(startsWith(app_ver, "01.11")))
					{
						DoPatch_GravityDaze2_111(app_pid, text_base);
					}
					else if ((startsWith(app_id, "PCAS00035") ||
							  startsWith(app_id, "PCJS50004") ||
							  startsWith(app_id, "PCJS50008") ||
							  startsWith(app_id, "PCJS66015") ||
							  startsWith(app_id, "CUSA00546") ||
							  startsWith(app_id, "CUSA01112") ||
							  startsWith(app_id, "CUSA01113") ||
							  startsWith(app_id, "CUSA01130") ||
							  startsWith(app_id, "CUSA02318")) &&
							 (startsWith(app_ver, "01.00")))
					{
						DoPatchGravityDaze_101(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA00900") ||
							  startsWith(app_id, "CUSA00207") ||
							  startsWith(app_id, "CUSA01363") ||
							  startsWith(app_id, "CUSA03173") ||
							  startsWith(app_id, "CUSA03023") ||
							  startsWith(app_id, "CUSA00208") ||
							  startsWith(app_id, "CUSA01363")) &&
							 (startsWith(app_ver, "01.09")))
					{
						DoPatch_Bloodborne109(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA00003") ||
							  startsWith(app_id, "CUSA00064") ||
							  startsWith(app_id, "CUSA00066") ||
							  startsWith(app_id, "CUSA00093") ||
							  startsWith(app_id, "CUSA00879")) &&
							 (startsWith(app_ver, "01.28")))
					{
						DoPatch_DriveClub_128(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA03627") ||
							  startsWith(app_id, "CUSA03745") ||
							  startsWith(app_id, "CUSA04936")) &&
							 (startsWith(app_ver, "01.03")))
					{
						DoPatch_TheLastGuardian_103(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA08034") ||
							  startsWith(app_id, "CUSA08804") ||
							  startsWith(app_id, "CUSA08809")) &&
							 (startsWith(app_ver, "01.00") ||
							  startsWith(app_ver, "01.01")))
					{
						DoPatch_SOTC_100(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA07820") ||
							  startsWith(app_id, "CUSA10249") ||
							  startsWith(app_id, "CUSA13986") ||
							  startsWith(app_id, "CUSA14006")) &&
							 (startsWith(app_ver, "01.00")))
					{
						DoPatch_t2ps4(app_pid, text_base, 0x100);
					}
					else if ((startsWith(app_id, "CUSA07820") ||
							  startsWith(app_id, "CUSA10249") ||
							  startsWith(app_id, "CUSA13986") ||
							  startsWith(app_id, "CUSA14006")) &&
							 (startsWith(app_ver, "01.09")))
					{
						DoPatch_t2ps4(app_pid, text_base, 0x109);
					}
					else if ((startsWith(app_id, "CUSA00552") ||
							  startsWith(app_id, "CUSA00556") ||
							  startsWith(app_id, "CUSA00557") ||
							  startsWith(app_id, "CUSA00559")) &&
							 (startsWith(app_ver, "01.11")))
					{
						DoPatch_t1ps4_111(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA09254") ||
							  startsWith(app_id, "CUSA09264") ||
							  startsWith(app_id, "CUSA12635") ||
							  startsWith(app_id, "CUSA13217") ||
							  startsWith(app_id, "CUSA13318")) &&
							 (startsWith(app_ver, "01.32")))
					{
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x0264c85d), "41be00000000");
						write_bytes(app_pid, NO_ASLR(0x005b6bcd), "41be01000000");
						write_bytes(app_pid, NO_ASLR(0x005b6bd3), "eb1a");
						// res patch
						/*
						write_bytes(app_pid, NO_ASLR(0x026533ef), "66c781386800000000");
						write_bytes(app_pid, NO_ASLR(0x026533f8), "c6813a68000000");
						write_bytes32(app_pid, NO_ASLR(0x02653426), 3840);
						write_bytes32(app_pid, NO_ASLR(0x02653441), 2160);
						write_bytes32(app_pid, NO_ASLR(0x026565f6), 3840);
						write_bytes32(app_pid, NO_ASLR(0x026565e1), 2160);
						write_bytes32(app_pid, NO_ASLR(0x026533cc), 3840);
						write_bytes32(app_pid, NO_ASLR(0x026533d0), 2160);
						*/
						// cheats
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA01493") ||
							  startsWith(app_id, "CUSA02747") ||
							  startsWith(app_id, "CUSA02748")) &&
							 (startsWith(app_ver, "01.05")))
					{
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x00a1f817), "4831c9");
						write_bytes(app_pid, NO_ASLR(0x01ddd118), "41c7c403000000");
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA00133") ||
							  startsWith(app_id, "CUSA00135") ||
							  startsWith(app_id, "CUSA01009")) &&
							 (startsWith(app_ver, "01.15")))
					{
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x009fa57e), "be00000000");
						write_bytes(app_pid, NO_ASLR(0x009fa596), "b800000000");   // vsync
						write_bytes(app_pid, NO_ASLR(0x009fb9e1), "48e9a9000000"); // no GUseFixedTimeStep
						write_bytes(app_pid, NO_ASLR(0x017b2a62), "c5f857c09090"); // No MaxSmoothedFPS, fixes 58 fps
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA18097") ||
							  startsWith(app_id, "CUSA18100") ||
							  startsWith(app_id, "CUSA19278")) &&
							 (startsWith(app_ver, "01.04")))
					{
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x0312a29a), "418b7c240831f60f1f8000000000"); // fliprate
						// write_bytes(app_pid, NO_ASLR(0x0311ea1b), "b801000000"); // use vsync on base ps4 // redundant?
						write_bytes32(app_pid, NO_ASLR(0x045a1a10), 0x3c888889);								 // menu
						write_bytes(app_pid, NO_ASLR(0x01be8ba9), "41c7471808000000660f1f4400000f1f8000000000"); // gameplay
						write_bytes32(app_pid, NO_ASLR(0x045f6f28), 0);											 // logo movies
						write_bytes32(app_pid, NO_ASLR(0x045f6f3d), 0);
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA00341") ||
							  startsWith(app_id, "CUSA00912") ||
							  startsWith(app_id, "CUSA00917") ||
							  startsWith(app_id, "CUSA00918") ||
							  startsWith(app_id, "CUSA04529")) &&
							 (startsWith(app_ver, "01.33")))
					{
						// 60 FPS
						write_bytes32(app_pid, NO_ASLR(0x00616e54), 0x1);
						write_bytes(app_pid, NO_ASLR(0x020e9880), "31f6");
						write_bytes(app_pid, NO_ASLR(0x020e9882), "ff25804e4600");
						// Debug Menu
						write_bytes(app_pid, NO_ASLR(0x005c8d97), "c7c101000000");
						write_bytes(app_pid, NO_ASLR(0x005c8d9d), "41b401");
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA07875") ||
							  startsWith(app_id, "CUSA09564") ||
							  startsWith(app_id, "CUSA07737") ||
							  startsWith(app_id, "CUSA08347") ||
							  startsWith(app_id, "CUSA08352")) &&
							 (startsWith(app_ver, "01.09")))
					{
						// 60 FPS
						write_bytes32(app_pid, NO_ASLR(0x00623bff), 0x1);
						write_bytes(app_pid, NO_ASLR(0x022193f0), "31f6");
						write_bytes(app_pid, NO_ASLR(0x022193f2), "ff25b8484800");
						// Debug Menu
						write_bytes(app_pid, NO_ASLR(0x005c8fc3), "31c9");
						write_bytes(app_pid, NO_ASLR(0x005c8fc5), "0f1f4000");
						write_bytes(app_pid, NO_ASLR(0x005c8fd6), "31d2");
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA00663") ||
							  startsWith(app_id, "CUSA00605") ||
							  startsWith(app_id, "CUSA00476")) &&
							 (startsWith(app_ver, "01.05")))
					{
						// Startup logo skip
						write_bytes(app_pid, NO_ASLR(0x03ffaadb), "0f85");
						printf_notification("%s (%s): 60 FPS Patched!", app->titleId().c_str(), app_ver);
					}
					else if ((startsWith(app_id, "CUSA00049") ||
							  startsWith(app_id, "CUSA00110") ||
							  startsWith(app_id, "CUSA00157")) &&
							 (startsWith(app_ver, "01.24")))
					{
						DoPatchBF4_124(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA00002") ||
							  startsWith(app_id, "CUSA00008") ||
							  startsWith(app_id, "CUSA00085") ||
							  startsWith(app_id, "CUSA00190")) &&
							 (startsWith(app_ver, "01.81")))
					{
						DoPatchKillzone_181(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA01499") ||
							  startsWith(app_id, "CUSA01542") ||
							  startsWith(app_id, "CUSA01566")) &&
							 (startsWith(app_ver, "01.02")))
					{
						DoPatchMEC_102(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA00220") ||
							  startsWith(app_id, "CUSA00503") ||
							  startsWith(app_id, "CUSA00728") ||
							  startsWith(app_id, "CUSA01425")) &&
							 (startsWith(app_ver, "01.12")))
					{
						// 60 FPS
						write_bytes(app_pid, NO_ASLR(0x015dcc1f), "be00000000");
						write_bytes(app_pid, NO_ASLR(0x012517d1), "c744202800007042");
						write_bytes(app_pid, NO_ASLR(0x012517d9), "c644205201");
						write_bytes(app_pid, NO_ASLR(0x015dcc5f), "b800000000");
						printf_notification("%s (%s): 60 FPS Patched!", app_id, app_ver);
					}
					else if ((startsWith(app_id, "CUSA18471") ||
							  startsWith(app_id, "CUSA18742") ||
							  startsWith(app_id, "CUSA18774") ||
							  startsWith(app_id, "CUSA19072")) &&
							 (startsWith(app_ver, "01.03")))
					{
						DoPatchNier103(app_pid, text_base);
					}
					else if (((startsWith(app_id, "CUSA02085") ||
							  startsWith(app_id, "CUSA02092")) &&
							 (startsWith(app_ver, "01.12"))) ||
							 (startsWith(app_id, "CUSA04701") && startsWith(app_ver, "01.11")))
					{
						DoPatchDoom_112(app_pid, text_base);
					}
					else if ((startsWith(app_id, "CUSA10866") ||
							  startsWith(app_id, "CUSA10851")) &&
							 (startsWith(app_ver, "01.03")))
					{
						DoPatchBPR_103(app_pid, text_base);
					}
				}

				// multiple selfs
				// this thread wait model is stinky
				// big2
				if ((startsWith(app_id, "CUSA01399") ||
					 startsWith(app_id, "CUSA02320") ||
					 startsWith(app_id, "CUSA02343") ||
					 startsWith(app_id, "CUSA02344") ||
					 startsWith(app_id, "CUSA02826")))
				{
					if (startsWith(app_ver, "01.00"))
					{
						if (startsWith(process_name_c_str, "eboot.bin"))
						{
							DoPatch_BigCollection(app_pid, text_base, (0x100 << 1));
						}
						else if (startsWith(process_name_c_str, "big2-ps4_Shipping.elf"))
						{
							DoPatch_BigCollection(app_pid, text_base, (0x100 << 2));
						}
						// big3
						else if (startsWith(process_name_c_str, "big3-ps4_Shipping.elf"))
						{
							DoPatch_BigCollection(app_pid, text_base, (0x100 << 3));
						}
					}
					else if (startsWith(app_ver, "01.02"))
					{
						if (startsWith(process_name_c_str, "eboot.bin"))
						{
							DoPatch_BigCollection(app_pid, text_base, (0x102 << 1));
						}
						else if (startsWith(process_name_c_str, "big2-ps4_Shipping.elf"))
						{
							DoPatch_BigCollection(app_pid, text_base, (0x102 << 2));
						}
						// big3
						else if (startsWith(process_name_c_str, "big3-ps4_Shipping.elf"))
						{
							DoPatch_BigCollection(app_pid, text_base, (0x102 << 3));
						}
					}
				}
				int32_t fliprate_game_found = false;
				if (Xml_parseTitleID(app_id))
				{
					printf_notification("Title ID found in universal fliprate list:\n%s", app_id);
					ResumeApp(app_pid);
					patch_SetFlipRate(*executable, app_pid);
					fliprate_game_found = true;
					break;
				}
				if (!fliprate_game_found)
				{
					ResumeApp(app_pid);
					if (g_UniversalFlipRatePatch)
					{
						patch_SetFlipRate(*executable, app_pid);
					}
				}
			}
			else if (text_base && !g_foundApp && (startsWith(app_id, "PPSA")))
			{
				char app_ver[APP_VER_SIZE]{};		// `contentVersion`
				char master_ver[MASTER_VER_SIZE]{}; // `masterVersion`
				char content_id[CONTENT_ID_SIZE]{}; // `contentId`
				int32_t ret = get_app_info(app_id, app_ver, master_ver, content_id, PS5_APP);
				if (ret != 0)
				{
					// something went wrong
					printf_notification("get_app_info(%s) failed! %d", app_id, ret);
					continue;
				}
				else if (ret == 0)
				{
					g_foundApp = true;
					target_running_pid = app_pid;
				}
				SuspendApp(app_pid);
				// eboot.bin games
				if (startsWith(process_name_c_str, "eboot.bin"))
				{
					if ((startsWith(app_id, "PPSA01339") ||
						 startsWith(app_id, "PPSA01340") ||
						 startsWith(app_id, "PPSA01341") ||
						 startsWith(app_id, "PPSA01342")))
					{
						if (startsWith(app_ver, "01.000.000"))
						{
							DoPatch_DemonSouls(app_pid, text_base, 0x100);
						}
						else if (startsWith(app_ver, "01.004.000"))
						{
							DoPatch_DemonSouls(app_pid, text_base, 0x104);
						}
					}
				}
				if ((startsWith(app_id, "PPSA05684") ||
					 startsWith(app_id, "PPSA05389") ||
					 startsWith(app_id, "PPSA05686") ||
					 startsWith(app_id, "PPSA05685")) &&
					(startsWith(app_ver, "01.000.000")))
				{
					if (startsWith(process_name_c_str, "eboot.bin"))
					{
						DoPatch_Big4R_100(app_pid, text_base, 1);
						printf_notification("%s (%s): Debug Menu Patched!", app_id, app_ver);
					}
					// tll big4r
					else if (startsWith(process_name_c_str, "tllr-boot.bin"))
					{
						DoPatch_Big4R_100(app_pid, text_base, 2);
						printf_notification("%s (%s): Debug Menu Patched!", app_id, process_name_c_str);
					}
				}
				ResumeApp(app_pid);
				if (g_UniversalFlipRatePatch)
				{
					patch_SetFlipRate(*executable, app_pid);
				}
			}
		}
		if (target_running_pid > 0 && !isAlive(target_running_pid))
		{
			target_running_pid = -1;
			g_foundApp = false;
		}
	}

	if (shellcore_pid)
	{
		if (UnPatchShellCore(shellcore_pid))
		{
			printf_notification("Patches for ShellCore has been uninstalled.");
		}
		else
		{
			printf_notification("Failed to uninstall patches for ShellCore.");
		}
	}
	printf_notification("Game Patch thread has requested to stop");
	pthread_exit(nullptr);
	return nullptr;
}

#undef APP_VER_SIZE
#undef MASTER_VER_SIZE
#undef CONTENT_ID_SIZE

static void TestCallback(void *args) {
	(void) args;
}

static constexpr __attribute__ ((used)) TitleIdMap TITLEID_HANDLERS{{
	{TestCallback, "BREW00000"_tid},
	{TestCallback, {"BREW00001"}}
}};
