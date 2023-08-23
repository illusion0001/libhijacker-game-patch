#include <netinet/in.h>
#include <poll.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "fd.hpp"
#include "hijacker/hijacker.hpp"
#include "servers.hpp"
#include "util.hpp"
#include "notify.hpp"

#include <pthread.h>
#include "game_patch_thread.hpp"

#include "print.hpp"

extern void makenewapp();

void AbortServer::run(TcpSocket &sock) {
	// any connection signals to shutdown the daemon
	puts("abort signal received");
	sock.close();
}

struct HookBuilder {
	static constexpr size_t CODE_SIZE = 92;
	//static constexpr size_t PRINTF_OFFSET = 0x31;
	//static constexpr size_t EXECVE_OFFSET = 0x4F;
	static constexpr size_t EXECVE_OFFSET = 0x4D;
	uint8_t data[CODE_SIZE];

	void setPrintf(uintptr_t addr) {
		//*reinterpret_cast<uintptr_t *>(data + PRINTF_OFFSET) = addr;
	}
	void setExecve(uintptr_t addr) {
		*reinterpret_cast<uintptr_t *>(data + EXECVE_OFFSET) = addr;
	}
};

// NOLINTBEGIN(*)

/*
int hookExecve(const char *path, const char **argv, const char **envp) {
	printf("execve %s\n", path);
	return execve(path, argv, envp);
}
*/
/*
static constexpr HookBuilder HOOKER{{
    0x48, 0x83, 0xec, 0x28, 0x48, 0xb8, 0x2f, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x5f, 0x48, 0xbf,
    0x65, 0x78, 0x2f, 0x61, 0x70, 0x70, 0x2f, 0x4e, 0x48, 0xb9, 0x50, 0x58, 0x53, 0x34, 0x30, 0x30,
    0x32, 0x38, 0x48, 0x89, 0x04, 0x24, 0x48, 0x89, 0x7c, 0x24, 0x08, 0x48, 0x89, 0xe7, 0x48, 0xb8,
    0x2f, 0x65, 0x62, 0x6f, 0x6f, 0x74, 0x2e, 0x62, 0x48, 0x89, 0x4c, 0x24, 0x10, 0x48, 0x89, 0x44,
    0x24, 0x18, 0x48, 0xc7, 0x44, 0x24, 0x20, 0x69, 0x6e, 0x00, 0x00, 0x48, 0xb8, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xd0, 0x48, 0x83, 0xc4, 0x28, 0xc3
}};

static constexpr HookBuilder HOOKER{{
	// PUSH R15
	0x41, 0x57,
	// PUSH R14
	0x41, 0x56,
	// PUSH RBX
	0x53,
	// SUB RSP, 0x10
	0x48, 0x83, 0xec, 0x10,
	// MOV R15, RDI
	0x49, 0x89, 0xff,
	// MOV RAX, 0x2520657663657865
	0x48, 0xb8, 0x65, 0x78, 0x65, 0x63, 0x76, 0x65, 0x20, 0x25,
	// MOV R14, RSI
	0x49, 0x89, 0xf6,
	// MOV RDI, RSP
	0x48, 0x89, 0xe7,
	// MOV RBX, RDX
	0x48, 0x89, 0xd3,
	// MOV qword ptr [RSP], RAX
	0x48, 0x89, 0x04, 0x24,
	// MOV RSI, R15
	0x4c, 0x89, 0xfe,
	// MOV qword ptr [RSP + 0x8], 0x6e5c73
	0x48, 0xc7, 0x44, 0x24, 0x08, 0x73, 0x5c, 0x6e, 0x00,
	// MOV RAX, printf
	0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// CALL RAX
	0xff, 0xd0,
	// MOV RDI, R15
	0x4c, 0x89, 0xff,
	// MOV RSI, R14
	0x4c, 0x89, 0xf6,
	// MOV RDX, RBX
	0x48, 0x89, 0xda,
	// ADD RSP, 0x10
	0x48, 0x83, 0xc4, 0x10,
	// POP RBX
	0x5b,
	// POP R14
	0x41, 0x5e,
	// POP R15
	0x41, 0x5f,
	// MOV RAX, execve
	0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	// JMP RAX
	0xff, 0xe0
}};
*/

// NOLINTEND(*)
/*
bool patchSyscore() {
	return true;
	puts("patching syscore execve");
	auto hijacker = Hijacker::getHijacker("SceSysCore.elf"_sv);
	if (hijacker == nullptr) {
		puts("failed to get SceSysCore.elf");
		return false;
	}
	uintptr_t code = hijacker->getTextAllocator().allocate(HookBuilder::CODE_SIZE);
	static constexpr Nid execveNid{"kdguLiAheLI"};
	//static constexpr Nid printfNid{"hcuQgD53UxM"};
	//static constexpr Nid amd64_set_fsbaseNid{"3SVaehJvYFk"};
	auto execve = hijacker->getLibKernelFunctionAddress(execveNid);
	if (execve == 0) {
		puts("failed to locate execve");
		return false;
	}
	auto meta = hijacker->getEboot()->getMetaData();
	const auto &plttab = meta->getPltTable();
	if (ELF64_R_TYPE(plttab[0].r_info) != R_X86_64_JMP_SLOT) {
		puts("wrong table stupid");
		return false;
	}
	auto index = meta->getSymbolTable().getSymbolIndex(execveNid);
	if (index == -1) {
		puts("execve import not found");
		return false;
	}
	*/
	/*
	auto fsbaseIndex = meta->getSymbolTable().getSymbolIndex(amd64_set_fsbaseNid);
	for (const auto &plt : plttab) {
		if (ELF64_R_SYM(plt.r_info) == fsbaseIndex) {
			auto retptr = hijacker->getTextAllocator().allocate(1);
			uint8_t ret[]{0xc3}; // NOLINT(*)
			hijacker->write(retptr, ret);
			uintptr_t addr = hijacker->getEboot()->imagebase() + plt.r_offset;
			hijacker->write<uintptr_t>(addr, retptr);
			break;
		}
	}

	for (const auto &plt : plttab) {
		if (ELF64_R_SYM(plt.r_info) == index) {
			HookBuilder hooker = HOOKER;
			auto printfAddr = hijacker->getFunctionAddress(hijacker->getLib(2).get(), printfNid);
			if (printfAddr == 0) {
				puts("failed to locate printf");
				return false;
			}
			hooker.setPrintf(printfAddr);
			hooker.setExecve(execve);
			hijacker->write(code, hooker.data);
			uintptr_t addr = hijacker->getEboot()->imagebase() + plt.r_offset;
			hijacker->write<uintptr_t>(addr, code);
			hexdump(hooker.data, HookBuilder::CODE_SIZE);
			return true;
		}
	}
	return false;
}*/

extern "C"
{

#include <stdint.h>

#define ORBIS_COMMON_DIALOG_MAGIC_NUMBER 0xC0D1A109

	typedef enum OrbisCommonDialogResult
	{
		ORBIS_COMMON_DIALOG_RESULT_OK = 0,
		ORBIS_COMMON_DIALOG_RESULT_USER_CANCELED = 1
	} OrbisCommonDialogResult;

	typedef struct OrbisCommonDialogBaseParam
	{
		size_t size;
		uint8_t reserved[36];
		uint32_t magic;
	} OrbisCommonDialogBaseParam __attribute__((__aligned__(8)));

	typedef enum OrbisCommonDialogStatus
	{
		ORBIS_COMMON_DIALOG_STATUS_NONE = 0,
		ORBIS_COMMON_DIALOG_STATUS_INITIALIZED = 1,
		ORBIS_COMMON_DIALOG_STATUS_RUNNING = 2,
		ORBIS_COMMON_DIALOG_STATUS_FINISHED = 3
	} OrbisCommonDialogStatus;

	typedef int32_t OrbisUserServiceUserId;
	typedef int32_t OrbisMsgDialogButtonId;
	typedef int32_t OrbisMsgDialogProgressBarTarget;

	typedef enum OrbisMsgDialogButton : OrbisMsgDialogButtonId
	{
		ORBIS_MSG_DIALOG_BUTTON_ID_INVALID = 0,
		ORBIS_MSG_DIALOG_BUTTON_ID_OK = 1,
		ORBIS_MSG_DIALOG_BUTTON_ID_YES = 1,
		ORBIS_MSG_DIALOG_BUTTON_ID_NO = 2,
		ORBIS_MSG_DIALOG_BUTTON_ID_BUTTON1 = 1,
		ORBIS_MSG_DIALOG_BUTTON_ID_BUTTON2 = 2,
	} OrbisMsgDialogButton;

	typedef enum OrbisMsgDialogMode
	{
		ORBIS_MSG_DIALOG_MODE_USER_MSG = 1,
		ORBIS_MSG_DIALOG_MODE_PROGRESS_BAR = 2,
		ORBIS_MSG_DIALOG_MODE_SYSTEM_MSG = 3,
	} OrbisMsgDialogMode;

	typedef struct OrbisMsgDialogResult
	{
		OrbisMsgDialogMode mode;
		int32_t result;
		OrbisMsgDialogButton buttonId;
		char reserved[32];
	} OrbisMsgDialogResult;

	typedef enum OrbisMsgDialogButtonType
	{
		ORBIS_MSG_DIALOG_BUTTON_TYPE_OK = 0,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_YESNO = 1,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_NONE = 2,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_OK_CANCEL = 3,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_WAIT = 5,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_WAIT_CANCEL = 6,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_YESNO_FOCUS_NO = 7,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_OK_CANCEL_FOCUS_CANCEL = 8,
		ORBIS_MSG_DIALOG_BUTTON_TYPE_2BUTTONS = 9,
	} OrbisMsgDialogButtonType;

	typedef enum OrbisMsgDialogProgressBarType
	{
		ORBIS_MSG_DIALOG_PROGRESSBAR_TYPE_PERCENTAGE = 0,
		ORBIS_MSG_DIALOG_PROGRESSBAR_TYPE_PERCENTAGE_CANCEL = 1,
	} OrbisMsgDialogProgressBarType;

	typedef enum OrbisMsgDialogSystemMessageType
	{
		ORBIS_MSG_DIALOG_SYSMSG_TYPE_TRC_EMPTY_STORE = 0,
		ORBIS_MSG_DIALOG_SYSMSG_TYPE_TRC_PSN_CHAT_RESTRICTION = 1,
		ORBIS_MSG_DIALOG_SYSMSG_TYPE_TRC_PSN_UGC_RESTRICTION = 2,
		ORBIS_MSG_DIALOG_SYSMSG_TYPE_CAMERA_NOT_CONNECTED = 4,
		ORBIS_MSG_DIALOG_SYSMSG_TYPE_WARNING_PROFILE_PICTURE_AND_NAME_NOT_SHARED = 5,
	} OrbisMsgDialogSystemMessageType;

	typedef struct OrbisMsgDialogButtonsParam
	{
		const char *msg1;
		const char *msg2;
		char reserved[32];
	} OrbisMsgDialogButtonsParam;

	typedef struct OrbisMsgDialogUserMessageParam
	{
		OrbisMsgDialogButtonType buttonType;
		int32_t : 32;
		const char *msg;
		OrbisMsgDialogButtonsParam *buttonsParam;
		char reserved[24];
	} OrbisMsgDialogUserMessageParam;

	typedef struct OrbisMsgDialogProgressBarParam
	{
		OrbisMsgDialogProgressBarType barType;
		int32_t : 32;
		const char *msg;
		char reserved[64];
	} OrbisMsgDialogProgressBarParam;

	typedef struct OrbisMsgDialogSystemMessageParam
	{
		OrbisMsgDialogSystemMessageType sysMsgType;
		char reserved[32];
	} OrbisMsgDialogSystemMessageParam;

	typedef struct OrbisMsgDialogParam
	{
		OrbisCommonDialogBaseParam baseParam;
		size_t size;
		OrbisMsgDialogMode mode;
		int32_t : 32;
		OrbisMsgDialogUserMessageParam *userMsgParam;
		OrbisMsgDialogProgressBarParam *progBarParam;
		OrbisMsgDialogSystemMessageParam *sysMsgParam;
		OrbisUserServiceUserId userId;
		char reserved[40];
		int32_t : 32;
	} OrbisMsgDialogParam;

	// Initialize the message dialog. Should be called before trying to use the
	// message dialog.
	int32_t sceMsgDialogInitialize(void);

	// Display the message dialog.
	int32_t sceMsgDialogOpen(const OrbisMsgDialogParam *param);

	// Get the result of the message dialog after the user closes the dialog.
	// This can be used to detect which option was pressed (yes, no, cancel, etc).
	int32_t sceMsgDialogGetResult(OrbisMsgDialogResult *result);

	// Get the status of the message dialog. This can be used to check if a
	// message dialog is initialized, is being displayed, or is finished.
	OrbisCommonDialogStatus sceMsgDialogGetStatus();

	// Update the current status of the message dialog.
	OrbisCommonDialogStatus sceMsgDialogUpdateStatus(void);

	// Increase the message dialog progress bar percentage.
	// OrbisMsgDialogMode must be initialized with ORBIS_MSG_DIALOG_MODE_PROGRESS_BAR.
	int32_t sceMsgDialogProgressBarInc(OrbisMsgDialogProgressBarTarget target, uint32_t delta);

	// Add a message to the message dialog progress bar.
	// OrbisMsgDialogMode must be initialized with ORBIS_MSG_DIALOG_MODE_PROGRESS_BAR.
	int32_t sceMsgDialogProgressBarSetMsg(OrbisMsgDialogProgressBarTarget target, const char *barMsg);

	// Set the message dialog progress bar immediately without animation.
	// OrbisMsgDialogMode must be initialized with ORBIS_MSG_DIALOG_MODE_PROGRESS_BAR.
	int32_t sceMsgDialogProgressBarSetValue(OrbisMsgDialogProgressBarTarget target, uint32_t rate);

	// Close the message dialog.
	int32_t sceMsgDialogClose(void);

	// Terminate the message dialog. Should be called when all message dialog
	// operations are finished.
	int32_t sceMsgDialogTerminate(void);

	int32_t sceSysmoduleLoadModule(uint16_t module);

	int32_t sceCommonDialogInitialize(void);
}

// Dialog types
#define MDIALOG_OK 0
#define MDIALOG_YESNO 1

static inline void _orbisCommonDialogSetMagicNumber(uint32_t *magic, const OrbisCommonDialogBaseParam *param)
{
	*magic = (uint32_t)(ORBIS_COMMON_DIALOG_MAGIC_NUMBER + (uint64_t)param);
}

static inline void _orbisCommonDialogBaseParamInit(OrbisCommonDialogBaseParam *param)
{
	(void)memset(param, 0x0, sizeof(OrbisCommonDialogBaseParam));
	param->size = (uint32_t)sizeof(OrbisCommonDialogBaseParam);
	_orbisCommonDialogSetMagicNumber(&(param->magic), param);
}

static inline void orbisMsgDialogParamInitialize(OrbisMsgDialogParam *param)
{
	(void)memset(param, 0x0, sizeof(OrbisMsgDialogParam));
	_orbisCommonDialogBaseParamInit(&param->baseParam);
	param->size = sizeof(OrbisMsgDialogParam);
}

int show_dialog(int dialog_type, const char *format, ...)
{
	OrbisMsgDialogParam param{};
	OrbisMsgDialogUserMessageParam userMsgParam{};
	OrbisMsgDialogResult result{};

	char str[2048]{};

	va_list opt{};
	va_start(opt, format);
	vsprintf(str, format, opt);
	va_end(opt);

	_puts(str);

	print_ret(sceMsgDialogInitialize());
	orbisMsgDialogParamInitialize(&param);
	param.mode = ORBIS_MSG_DIALOG_MODE_USER_MSG;

	// memset(&userMsgParam, 0, sizeof(userMsgParam));
	userMsgParam.msg = str;
	userMsgParam.buttonType = (dialog_type ? ORBIS_MSG_DIALOG_BUTTON_TYPE_YESNO_FOCUS_NO : ORBIS_MSG_DIALOG_BUTTON_TYPE_OK);
	param.userMsgParam = &userMsgParam;

	int32_t open_ret = sceMsgDialogOpen(&param);
	_printf("sceMsgDialogOpen: 0x%08x\n", open_ret);
	if (open_ret < 0)
		return 0;

	do
	{
	} while (sceMsgDialogUpdateStatus() != ORBIS_COMMON_DIALOG_STATUS_FINISHED);
	sceMsgDialogClose();

	sceMsgDialogGetResult(&result);
	sceMsgDialogTerminate();

	return (result.buttonId == ORBIS_MSG_DIALOG_BUTTON_ID_YES);
}

void InitMsg(void)
{
	constexpr uint16_t ORBIS_SYSMODULE_MESSAGE_DIALOG = 0x00A4;
	int32_t ret_1 = sceSysmoduleLoadModule(ORBIS_SYSMODULE_MESSAGE_DIALOG);
	int32_t ret_2 = sceCommonDialogInitialize();
	_printf("sceSysmoduleLoadModule: 0x%08x\n", ret_1);
	_printf("sceCommonDialogInitialize: 0x%08x\n", ret_2);
	if (ret_1 < 0)
	{
		printf_notification("Failed to initialize CommonDialog");
		return;
	}
	show_dialog(MDIALOG_OK, "User likes %s :)", "OpenOrbis");
}

int main() {
	puts("daemon entered");
	InitMsg();
	printf_notification("libhijacker daemon started successfully.\n"
						"Original project: https://github.com/astrelsky/libhijacker");
	printf_notification("libhijacker - astrelsky\n"
						"Fork with Game Patch Support - illusion\n"
						"Check Readme for supported titles and it's versions.");
	// remove this when it's possible to load elf into games at boot
	pthread_t game_patch_thread_id = nullptr;
	pthread_create(&game_patch_thread_id, nullptr, GamePatch_Thread, nullptr);

	g_game_patch_thread_running = true;

	AbortServer abortServer{};
	KlogServer klogServer{};
	ElfServer elfServer{};
	CommandServer commandServer{};

	abortServer.TcpServer::run();
	klogServer.TcpServer::run();
	elfServer.TcpServer::run();
	commandServer.TcpServer::run();

	/*if (!patchSyscore()) {
		puts("failed to patch syscore execve");
	} else {
		puts("successfully patched syscore execve");
	}*/

	// finishes on connect
	abortServer.join();
	puts("abort thread finished");
	g_game_patch_thread_running = false;
	puts("g_game_patch_thread_running = false");
	commandServer.stop();
	puts("command server done");
	puts("stopping elf server");
	elfServer.stop();
	puts("elf server done");
	puts("stopping klog server");
	klogServer.stop();
	puts("klog server done");
	pthread_join(game_patch_thread_id, nullptr);
	puts("game patch thread finished");

	printf_notification("daemon exit");

	// TODO add elf loader with options for process name and type (daemon/game)
	// add whatever other crap people may want
	return 0;
}
