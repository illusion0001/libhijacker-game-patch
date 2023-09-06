#include "game_patch_memory.hpp"

extern uint32_t FlipRate_ConfigureOutput_Ptr;
extern uint32_t FlipRate_isVideoModeSupported_Ptr;
extern int32_t is120HzUsable;
extern int32_t isPatch120Hz;

void DoPatch_Bloodborne109(pid_t app_pid, u64 text_base);
void DoPatch_GravityDaze2_111(pid_t app_pid, u64 text_base);
void DoPatch_SOTC_100(pid_t app_pid, u64 text_base);
void DoPatch_BigCollection(pid_t app_pid, u64 text_base, u32 idx);
void DoPatch_TheOrder1886_102(pid_t app_pid, u64 text_base);
void DoPatch_Big4R_100(pid_t app_pid, u64 text_base, u32 idx);
void DoPatch_DemonSouls(pid_t app_pid, u64 text_base, u32 idx);
void DoPatch_DriveClub_128(pid_t app_pid, u64 text_base);
void DoPatch_TheLastGuardian_103(pid_t app_pid, u64 text_base);
void DoPatch_CTR_121(pid_t app_pid, u64 text_base, u32 idx);
void DoPatch_t1ps4_111(pid_t app_pid, u64 text_base);
void DoPatch_t2ps4(pid_t app_pid, u64 text_base, u32 idx);
void DoPatch_ACEZioCollection_102(pid_t app_pid, u64 text_base, u32 idx);
void DoPatchBF4_124(pid_t app_pid, u64 text_base);
void DoPatchKillzone_181(pid_t app_pid, u64 text_base);
void DoPatchGravityDaze_101(pid_t app_pid, u64 text_base);
void DoPatchMEC_102(pid_t app_pid, u64 text_base);
