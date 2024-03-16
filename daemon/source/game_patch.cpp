#include "game_patch_memory.hpp"
#include "game_patch.hpp"
#include "game_patch_xml.hpp"
#include "game_patch_xml_cfg.hpp"
#include "print.hpp"
#include "notify.hpp"

int32_t g_isPatch120Hz = 0;

void DoPatch_Bloodborne109(pid_t app_pid, uint64_t text_base)
{
	// skip logos
	write_bytes32(app_pid, NO_ASLR(0x04d99138), 0);
	write_bytes32(app_pid, NO_ASLR(0x04d99154), 0);
	write_bytes32(app_pid, NO_ASLR(0x04d9916e), 0);
	// Patch DLC Save Requirement
	write_bytes(app_pid, NO_ASLR(0x023b67b3), "49c7460801010101"); // <!-- with `SPDLCMESSENGER00` [0] +`SPDLCMESSENGER01` [1] +`SPEXPANSIONDLC03` [3] but [2] is 0 but lets unlock it anyways -->
	// 60 FPS by Lance
	if (parseXML(BB_60FPSKey))
	{
		write_bytes(app_pid, NO_ASLR(0x00fbc40f), "eb1d");
		write_bytes(app_pid, NO_ASLR(0x013d2e16), "eb19");
		write_bytes(app_pid, NO_ASLR(0x013d2e18), "4156");
		write_bytes(app_pid, NO_ASLR(0x013d2e1a), "41c746080000003f");
		write_bytes(app_pid, NO_ASLR(0x013d2e22), "c4c17a5e4608");
		write_bytes(app_pid, NO_ASLR(0x013d2e28), "415e");
		write_bytes(app_pid, NO_ASLR(0x013d2e2a), "e933070000");
		write_bytes(app_pid, NO_ASLR(0x013d2e2f), "90");
		write_bytes(app_pid, NO_ASLR(0x013d2e30), "90");
		write_bytes(app_pid, NO_ASLR(0x013d3557), "c4c17a104608");
		write_bytes(app_pid, NO_ASLR(0x013d355d), "e9b6f8ffff");
		// bad in fixed timetep
		write_bytes(app_pid, NO_ASLR(0x01bf9b71), "e933010000");
		write_bytes(app_pid, NO_ASLR(0x01bf9b76), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9b77), "90");
		// end
		write_bytes(app_pid, NO_ASLR(0x01bf9ca9), "488d054868d403");
		write_bytes(app_pid, NO_ASLR(0x01bf9cb0), "488b00");
		write_bytes(app_pid, NO_ASLR(0x01bf9cb3), "8b8064020000");
		write_bytes(app_pid, NO_ASLR(0x01bf9cb9), "8945b8");
		write_bytes(app_pid, NO_ASLR(0x01bf9cbc), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cbd), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cbe), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cbf), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc0), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc1), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc2), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc3), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc4), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc5), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc6), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc7), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc8), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cc9), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cca), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9ccb), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9ccc), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9ccd), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cce), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9ccf), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cd0), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cd1), "90");
		write_bytes(app_pid, NO_ASLR(0x01bf9cd2), "e9a1feffff");
		write_bytes(app_pid, NO_ASLR(0x021bc181), "488d3d70437803");
		write_bytes(app_pid, NO_ASLR(0x021bc188), "488b3f");
		write_bytes(app_pid, NO_ASLR(0x021bc18b), "c5fa598764020000");
		write_bytes(app_pid, NO_ASLR(0x021bc193), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc194), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc195), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc196), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc197), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc198), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc199), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc19a), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc19b), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc19c), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc19d), "e96e010000");
		write_bytes(app_pid, NO_ASLR(0x021bc308), "e974feffff");
		write_bytes(app_pid, NO_ASLR(0x021bc30d), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc30e), "90");
		write_bytes(app_pid, NO_ASLR(0x021bc30f), "90");
		write_bytes(app_pid, NO_ASLR(0x02377cea), "eb24");
		write_bytes(app_pid, NO_ASLR(0x02377cec), "488d1d05885c03");
		write_bytes(app_pid, NO_ASLR(0x02377cf3), "488b1b");
		write_bytes(app_pid, NO_ASLR(0x02377cf6), "c5fa108364020000");
		write_bytes(app_pid, NO_ASLR(0x02377cfe), "90");
		write_bytes(app_pid, NO_ASLR(0x02377cff), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d00), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d01), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d02), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d03), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d04), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d05), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d06), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d07), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d08), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d09), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d0a), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d0b), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d0c), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d0d), "90");
		write_bytes(app_pid, NO_ASLR(0x02377d0e), "eb42");
		write_bytes(app_pid, NO_ASLR(0x02377d48), "eba2");
		write_bytes(app_pid, NO_ASLR(0x02418e3d), "8b8364020000");
		write_bytes(app_pid, NO_ASLR(0x02418e43), "8945c8");
		write_bytes(app_pid, NO_ASLR(0x02418e46), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e47), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e48), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e49), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e4a), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e4b), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e4c), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e4d), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e4e), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e4f), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e50), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e51), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e52), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e53), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e54), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e55), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e56), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e57), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e58), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e59), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e5a), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e5b), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e5c), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e5d), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e5e), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e5f), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e60), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e61), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e62), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e63), "90");
		write_bytes(app_pid, NO_ASLR(0x02418e64), "488d05a5765203");
		write_bytes(app_pid, NO_ASLR(0x0243487e), "41c74424188988883c");
		write_bytes(app_pid, NO_ASLR(0x02434887), "48b90000000001000000");
		write_bytes(app_pid, NO_ASLR(0x02483ec1), "c3");
		write_bytes(app_pid, NO_ASLR(0x02483ec2), "90");
		write_bytes(app_pid, NO_ASLR(0x02483ec3), "90");
		write_bytes(app_pid, NO_ASLR(0x02483ec4), "90");
		write_bytes(app_pid, NO_ASLR(0x02483ec5), "90");
		write_bytes(app_pid, NO_ASLR(0x02715d71), "e881090000");
		write_bytes(app_pid, NO_ASLR(0x02715d76), "90");
		write_bytes(app_pid, NO_ASLR(0x02715d77), "90");
		write_bytes(app_pid, NO_ASLR(0x02715d78), "90");
		write_bytes(app_pid, NO_ASLR(0x020367c7), "488d351cd2c702");
		write_bytes(app_pid, NO_ASLR(0x020369e8), "488d35b5cfc702");
		write_wstring(app_pid, NO_ASLR(0x04cb39ea), L"This 60fps patch was");
		write_wstring(app_pid, NO_ASLR(0x04cb39a4), L"Made with love, by Lance McDonald");
		// remove call to `assert()`
		write_bytes(app_pid, NO_ASLR(0x02fbf178), "4831c0c3");
		printf_notification("Bloodborne: 60 FPS Patched");
	}
	// no motion blur
	if (parseXML(BB_MBKey))
	{
		write_bytes(app_pid, NO_ASLR(0x026a057b), "eb16");
		printf_notification("Bloodborne: Motion Blur Patched");
	}
	// no Chromatic Aberration
	if (parseXML(BB_CAKey))
	{
		write_bytes(app_pid, NO_ASLR(0x0269faa8), "c783ac000000000000009090");
		printf_notification("Bloodborne: Chromatic Aberration Patched");
	}
	// Debug Camera
	if (parseXML(BB_DebugCameraKey))
	{
		write_bytes(app_pid, NO_ASLR(0x01972998), "7416");
		write_bytes(app_pid, NO_ASLR(0x0197299a), "48394a20");
		write_bytes(app_pid, NO_ASLR(0x0197299e), "4889c1");
		write_bytes(app_pid, NO_ASLR(0x019729a1), "7f03");
		write_bytes(app_pid, NO_ASLR(0x019729a3), "4889d1");
		write_bytes(app_pid, NO_ASLR(0x019729a6), "4839c1");
		write_bytes(app_pid, NO_ASLR(0x019729a9), "7405");
		write_bytes(app_pid, NO_ASLR(0x019729ab), "e979094700");
		write_bytes(app_pid, NO_ASLR(0x019729b0), "90");
		write_bytes(app_pid, NO_ASLR(0x019729b1), "90");
		write_bytes(app_pid, NO_ASLR(0x019729b2), "90");
		write_bytes(app_pid, NO_ASLR(0x019729b3), "31f6");
		write_bytes(app_pid, NO_ASLR(0x019729b5), "e8f63ad9ff");
		write_bytes(app_pid, NO_ASLR(0x019729ba), "e972094700");
		write_bytes(app_pid, NO_ASLR(0x01de3328), "c3");
		write_bytes(app_pid, NO_ASLR(0x01de3329), "c6413001");
		write_bytes(app_pid, NO_ASLR(0x01de332d), "488b4128");
		write_bytes(app_pid, NO_ASLR(0x01de3331), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3332), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3333), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3334), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3335), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3336), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3337), "50");
		write_bytes(app_pid, NO_ASLR(0x01de3338), "be08000000");
		write_bytes(app_pid, NO_ASLR(0x01de333d), "4889c7");
		write_bytes(app_pid, NO_ASLR(0x01de3340), "e81be95aff");
		write_bytes(app_pid, NO_ASLR(0x01de3345), "84c0");
		write_bytes(app_pid, NO_ASLR(0x01de3347), "0f84ff000000");
		write_bytes(app_pid, NO_ASLR(0x01de334d), "4180bc24c400000001");
		write_bytes(app_pid, NO_ASLR(0x01de3356), "7416");
		write_bytes(app_pid, NO_ASLR(0x01de3358), "41c68424c400000001");
		write_bytes(app_pid, NO_ASLR(0x01de3361), "41c6855001000001");
		write_bytes(app_pid, NO_ASLR(0x01de3369), "e929000000");
		write_bytes(app_pid, NO_ASLR(0x01de336e), "4180bd5001000000");
		write_bytes(app_pid, NO_ASLR(0x01de3376), "7413");
		write_bytes(app_pid, NO_ASLR(0x01de3378), "41c6855001000000");
		write_bytes(app_pid, NO_ASLR(0x01de3380), "41c68424c800000001");
		write_bytes(app_pid, NO_ASLR(0x01de3389), "eb0c");
		write_bytes(app_pid, NO_ASLR(0x01de338b), "49c78424c400000000000000");
		write_bytes(app_pid, NO_ASLR(0x01de3397), "498b8c24d0000000");
		write_bytes(app_pid, NO_ASLR(0x01de339f), "41807c244800");
		write_bytes(app_pid, NO_ASLR(0x01de33a5), "0f844c000000");
		write_bytes(app_pid, NO_ASLR(0x01de33ab), "c4c17828442450");
		write_bytes(app_pid, NO_ASLR(0x01de33b2), "c5f8294110");
		write_bytes(app_pid, NO_ASLR(0x01de33b7), "c4c17828442460");
		write_bytes(app_pid, NO_ASLR(0x01de33be), "c5f8294120");
		write_bytes(app_pid, NO_ASLR(0x01de33c3), "c4c17828442470");
		write_bytes(app_pid, NO_ASLR(0x01de33ca), "c5f8294130");
		write_bytes(app_pid, NO_ASLR(0x01de33cf), "c4c17828842480000000");
		write_bytes(app_pid, NO_ASLR(0x01de33d9), "c5f8294140");
		write_bytes(app_pid, NO_ASLR(0x01de33de), "498b8c24d0000000");
		write_bytes(app_pid, NO_ASLR(0x01de33e6), "c4c17a1044244c");
		write_bytes(app_pid, NO_ASLR(0x01de33ed), "c5fa114150");
		write_bytes(app_pid, NO_ASLR(0x01de33f2), "e955000000");
		write_bytes(app_pid, NO_ASLR(0x01de33f7), "498b542410");
		write_bytes(app_pid, NO_ASLR(0x01de33fc), "c5fa104250");
		write_bytes(app_pid, NO_ASLR(0x01de3401), "c5fa114150");
		write_bytes(app_pid, NO_ASLR(0x01de3406), "c5fa104254");
		write_bytes(app_pid, NO_ASLR(0x01de340b), "c5fa114154");
		write_bytes(app_pid, NO_ASLR(0x01de3410), "c5fa104258");
		write_bytes(app_pid, NO_ASLR(0x01de3415), "c5fa114158");
		write_bytes(app_pid, NO_ASLR(0x01de341a), "c5fa10425c");
		write_bytes(app_pid, NO_ASLR(0x01de341f), "c5fa11415c");
		write_bytes(app_pid, NO_ASLR(0x01de3424), "c5f8284210");
		write_bytes(app_pid, NO_ASLR(0x01de3429), "c5f8284a20");
		write_bytes(app_pid, NO_ASLR(0x01de342e), "c5f8285230");
		write_bytes(app_pid, NO_ASLR(0x01de3433), "c5f8285a40");
		write_bytes(app_pid, NO_ASLR(0x01de3438), "c5f8294110");
		write_bytes(app_pid, NO_ASLR(0x01de343d), "c5f8294920");
		write_bytes(app_pid, NO_ASLR(0x01de3442), "c5f8295130");
		write_bytes(app_pid, NO_ASLR(0x01de3447), "c5f8295940");
		write_bytes(app_pid, NO_ASLR(0x01de344c), "4180bc24c400000001");
		write_bytes(app_pid, NO_ASLR(0x01de3455), "7513");
		write_bytes(app_pid, NO_ASLR(0x01de3457), "4180bc24c800000000");
		write_bytes(app_pid, NO_ASLR(0x01de3460), "7508");
		write_bytes(app_pid, NO_ASLR(0x01de3462), "41c6855001000001");
		write_bytes(app_pid, NO_ASLR(0x01de346a), "58");
		write_bytes(app_pid, NO_ASLR(0x01de346b), "be41000000");
		write_bytes(app_pid, NO_ASLR(0x01de3470), "4889c7");
		write_bytes(app_pid, NO_ASLR(0x01de3473), "e8e8e75aff");
		write_bytes(app_pid, NO_ASLR(0x01de3478), "84c0");
		write_bytes(app_pid, NO_ASLR(0x01de347a), "7408");
		write_bytes(app_pid, NO_ASLR(0x01de347c), "41c6855001000000");
		write_bytes(app_pid, NO_ASLR(0x01de3484), "4180bd5001000000");
		write_bytes(app_pid, NO_ASLR(0x01de348c), "740a");
		write_bytes(app_pid, NO_ASLR(0x01de348e), "41c6855101000001");
		write_bytes(app_pid, NO_ASLR(0x01de3496), "eb08");
		write_bytes(app_pid, NO_ASLR(0x01de3498), "41c6855101000000");
		write_bytes(app_pid, NO_ASLR(0x01de34a0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34a9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34aa), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ab), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ac), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ad), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ae), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34af), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34b9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ba), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34bb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34bc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34bd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34be), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34bf), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34c9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ca), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34cb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34cc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34cd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ce), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34cf), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34d9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34da), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34db), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34dc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34dd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34de), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34df), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34e9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ea), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34eb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ec), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ed), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ee), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ef), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34f9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34fa), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34fb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34fc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34fd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34fe), "90");
		write_bytes(app_pid, NO_ASLR(0x01de34ff), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3500), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3501), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3502), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3503), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3504), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3505), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3506), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3507), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3508), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3509), "90");
		write_bytes(app_pid, NO_ASLR(0x01de350a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de350b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de350c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de350d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de350e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de350f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3510), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3511), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3512), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3513), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3514), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3515), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3516), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3517), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3518), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3519), "90");
		write_bytes(app_pid, NO_ASLR(0x01de351a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de351b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de351c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de351d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de351e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de351f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3520), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3521), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3522), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3523), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3524), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3525), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3526), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3527), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3528), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3529), "90");
		write_bytes(app_pid, NO_ASLR(0x01de352a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de352b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de352c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de352d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de352e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de352f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3530), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3531), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3532), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3533), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3534), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3535), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3536), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3537), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3538), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3539), "90");
		write_bytes(app_pid, NO_ASLR(0x01de353a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de353b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de353c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de353d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de353e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de353f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3540), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3541), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3542), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3543), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3544), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3545), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3546), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3547), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3548), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3549), "90");
		write_bytes(app_pid, NO_ASLR(0x01de354a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de354b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de354c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de354d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de354e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de354f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3550), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3551), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3552), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3553), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3554), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3555), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3556), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3557), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3558), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3559), "90");
		write_bytes(app_pid, NO_ASLR(0x01de355a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de355b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de355c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de355d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de355e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de355f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3560), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3561), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3562), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3563), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3564), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3565), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3566), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3567), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3568), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3569), "90");
		write_bytes(app_pid, NO_ASLR(0x01de356a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de356b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de356c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de356d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de356e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de356f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3570), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3571), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3572), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3573), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3574), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3575), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3576), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3577), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3578), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3579), "90");
		write_bytes(app_pid, NO_ASLR(0x01de357a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de357b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de357c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de357d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de357e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de357f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3580), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3581), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3582), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3583), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3584), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3585), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3586), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3587), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3588), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3589), "90");
		write_bytes(app_pid, NO_ASLR(0x01de358a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de358b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de358c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de358d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de358e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de358f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3590), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3591), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3592), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3593), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3594), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3595), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3596), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3597), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3598), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3599), "90");
		write_bytes(app_pid, NO_ASLR(0x01de359a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de359b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de359c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de359d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de359e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de359f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35a9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35aa), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ab), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ac), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ad), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ae), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35af), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35b9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ba), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35bb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35bc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35bd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35be), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35bf), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35c9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ca), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35cb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35cc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35cd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ce), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35cf), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35d9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35da), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35db), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35dc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35dd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35de), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35df), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35e9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ea), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35eb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ec), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ed), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ee), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ef), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f0), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f1), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f2), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f3), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f4), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f5), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f6), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f7), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f8), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35f9), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35fa), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35fb), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35fc), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35fd), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35fe), "90");
		write_bytes(app_pid, NO_ASLR(0x01de35ff), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3600), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3601), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3602), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3603), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3604), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3605), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3606), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3607), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3608), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3609), "90");
		write_bytes(app_pid, NO_ASLR(0x01de360a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de360b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de360c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de360d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de360e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de360f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3610), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3611), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3612), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3613), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3614), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3615), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3616), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3617), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3618), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3619), "90");
		write_bytes(app_pid, NO_ASLR(0x01de361a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de361b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de361c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de361d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de361e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de361f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3620), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3621), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3622), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3623), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3624), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3625), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3626), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3627), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3628), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3629), "90");
		write_bytes(app_pid, NO_ASLR(0x01de362a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de362b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de362c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de362d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de362e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de362f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3630), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3631), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3632), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3633), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3634), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3635), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3636), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3637), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3638), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3639), "90");
		write_bytes(app_pid, NO_ASLR(0x01de363a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de363b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de363c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de363d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de363e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de363f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3640), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3641), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3642), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3643), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3644), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3645), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3646), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3647), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3648), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3649), "90");
		write_bytes(app_pid, NO_ASLR(0x01de364a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de364b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de364c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de364d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de364e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de364f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3650), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3651), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3652), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3653), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3654), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3655), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3656), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3657), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3658), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3659), "90");
		write_bytes(app_pid, NO_ASLR(0x01de365a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de365b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de365c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de365d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de365e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de365f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3660), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3661), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3662), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3663), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3664), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3665), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3666), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3667), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3668), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3669), "90");
		write_bytes(app_pid, NO_ASLR(0x01de366a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de366b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de366c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de366d), "90");
		write_bytes(app_pid, NO_ASLR(0x01de366e), "90");
		write_bytes(app_pid, NO_ASLR(0x01de366f), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3670), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3671), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3672), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3673), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3674), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3675), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3676), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3677), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3678), "90");
		write_bytes(app_pid, NO_ASLR(0x01de3679), "90");
		write_bytes(app_pid, NO_ASLR(0x01de367a), "90");
		write_bytes(app_pid, NO_ASLR(0x01de367b), "90");
		write_bytes(app_pid, NO_ASLR(0x01de367c), "90");
		write_bytes(app_pid, NO_ASLR(0x01de367d), "4183bc24c400000001");
		write_bytes(app_pid, NO_ASLR(0x01de3686), "e934f3b8ff");
		write_bytes(app_pid, NO_ASLR(0x01de368b), "c3");
		printf_notification("Bloodborne: Debug Camera Patched");
	}
	// 1280x720 Resolution
	if (parseXML(BB_1280_720))
	{
		write_bytes32(app_pid, NO_ASLR(0x055289f8), 1280);
		write_bytes32(app_pid, NO_ASLR(0x055289fc), 720);
		printf_notification("Bloodborne: 720p Patched");
	}
	if (parseXML(BB_Vsync))
	{
		write_bytes(app_pid, NO_ASLR(0x025b3271), "4831f6");
		write_bytes(app_pid, NO_ASLR(0x0243487e), "41c74424186f12833a");
		printf_notification("Bloodborne: No Vsync Patched");
	}
	if (parseXML(BB_ColorBorder))
	{
		// <!-- Menu not focused -->
		write_bytes(app_pid, NO_ASLR(0x01401136), "eb");
		// <!-- No font drawing -->
		write_bytes(app_pid, NO_ASLR(0x01401168), "909090");
		write_bytes(app_pid, NO_ASLR(0x01402a90), "c3");
		write_bytes(app_pid, NO_ASLR(0x0140be00), "e98c6cffff");
		// <!-- Source -->
		/* <!--
						push rax
						push rsi
						mov qword ptr [rdi+0x28], 0 # (0,0)
						mov dword ptr [rdi+0x30], 0x42000000 (32,xxxx)
						lea rsi, [0x055289f8]
						mov eax, [rsi+0x4]         # vertical res
						vcvtsi2ss xmm0, xmm0, eax
						vmovss dword ptr [rdi+0x34], xmm0
						lea rax, [0x54dc040]
						MOV dword ptr [RDI+0x50], 0x3f800000
						MOV dword ptr [RDI+0x54], 0x3f800000
						MOV dword ptr [RDI+0x58], 0x3f800000
						MOV dword ptr [RDI+0x5c], 0x3f800000
						CMP dword ptr [RAX], 0xf   # above 16 index, reset
						JA color_default
						CMP dword ptr [RAX], 0x0   # check color index
						JZ color_0
						CMP dword ptr [RAX], 0x1
						JZ color_1
						CMP dword ptr [RAX], 0x2
						JZ color_2
						CMP dword ptr [RAX], 0x3
						JZ color_3
						CMP dword ptr [RAX], 0x4
						JZ color_4
						CMP dword ptr [RAX], 0x5
						JZ color_5
						CMP dword ptr [RAX], 0x6
						JZ color_6
						CMP dword ptr [RAX], 0x7
						JZ color_7
						CMP dword ptr [RAX], 0x8
						JZ color_8
						CMP dword ptr [RAX], 0x9
						JZ color_9
						CMP dword ptr [RAX], 0xa
						JZ color_10
						CMP dword ptr [RAX], 0xb
						JZ color_11
						CMP dword ptr [RAX], 0xc
						JZ color_12
						CMP dword ptr [RAX], 0xd
						JZ color_13
						CMP dword ptr [RAX], 0xe
						JZ color_14
						CMP dword ptr [RAX], 0xf
						JZ color_15
						color_default:
						# 255,255,255,255
						MOV dword ptr [RAX], 1                 # next frame, use second index
						JMP draw_bar_only_no_add
					color_0:
						# 255,255,255
						MOV dword ptr [RDI+0x50], 0x3f800000   # Red
						MOV dword ptr [RDI+0x54], 0x3f800000   # Green
						MOV dword ptr [RDI+0x58], 0x3f800000   # Blue
						JMP draw_bar
					color_1:
						# 0, 255, 0
						MOV dword ptr [RDI+0x50], 0x00000000   # Red
						MOV dword ptr [RDI+0x54], 0x3f800000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						JMP draw_bar
					color_2:
						# 0, 0, 255
						MOV dword ptr [RDI+0x50], 0x00000000   # Red
						MOV dword ptr [RDI+0x54], 0x00000000   # Green
						MOV dword ptr [RDI+0x58], 0x3f800000   # Blue
						JMP draw_bar
					color_3:
						# 255, 0, 0
						MOV dword ptr [RDI+0x50], 0x3f800000   # Red
						MOV dword ptr [RDI+0x54], 0x00000000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						JMP draw_bar
					color_4:
						# 0, 128, 128
						MOV dword ptr [RDI+0x50], 0x00000000   # Red
						MOV dword ptr [RDI+0x54], 0x3f000000   # Green
						MOV dword ptr [RDI+0x58], 0x3f000000   # Blue
						JMP draw_bar
					color_5:
						# 0, 0, 128
						MOV dword ptr [RDI+0x50], 0x00000000   # Red
						MOV dword ptr [RDI+0x54], 0x00000000   # Green
						MOV dword ptr [RDI+0x58], 0x3f000000   # Blue
						JMP draw_bar
					color_6:
						# 0, 128, 0
						MOV dword ptr [RDI+0x50], 0x00000000   # Red
						MOV dword ptr [RDI+0x54], 0x3f000000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						JMP draw_bar
					color_7:
						# 0, 255, 255
						MOV dword ptr [RDI+0x50], 0x00000000   # Red
						MOV dword ptr [RDI+0x54], 0x3f800000   # Green
						MOV dword ptr [RDI+0x58], 0x3f800000   # Blue
						JMP draw_bar
					color_8:
						# 128, 0, 0
						MOV dword ptr [RDI+0x50], 0x3f000000   # Red
						MOV dword ptr [RDI+0x54], 0x00000000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						JMP draw_bar
					color_9:
						# 192, 192, 192
						MOV dword ptr [RDI+0x50], 0x3f400000   # Red
						MOV dword ptr [RDI+0x54], 0x3f400000   # Green
						MOV dword ptr [RDI+0x58], 0x3f400000   # Blue
						JMP draw_bar
					color_10:
						# 128, 0, 128
						MOV dword ptr [RDI+0x50], 0x3f000000   # Red
						MOV dword ptr [RDI+0x54], 0x00000000   # Green
						MOV dword ptr [RDI+0x58], 0x3f000000   # Blue
						JMP draw_bar
					color_11:
						# 128, 128, 0
						MOV dword ptr [RDI+0x50], 0x3f000000   # Red
						MOV dword ptr [RDI+0x54], 0x3f000000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						JMP draw_bar
					color_12:
						# 128, 128, 128
						MOV dword ptr [RDI+0x50], 0x3f000000   # Red
						MOV dword ptr [RDI+0x54], 0x3f000000   # Green
						MOV dword ptr [RDI+0x58], 0x3f000000   # Blue
						JMP draw_bar
					color_13:
						# 255, 0, 255
						MOV dword ptr [RDI+0x50], 0x3f800000   # Red
						MOV dword ptr [RDI+0x54], 0x00000000   # Green
						MOV dword ptr [RDI+0x58], 0x3f800000   # Blue
						JMP draw_bar
					color_14:
						# 255, 255, 0
						MOV dword ptr [RDI+0x50], 0x3f800000   # Red
						MOV dword ptr [RDI+0x54], 0x3f800000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						JMP draw_bar
					color_15:
						# 255, 128, 0
						MOV dword ptr [RDI+0x50], 0x3f800000   # Red
						MOV dword ptr [RDI+0x54], 0x3f000000   # Green
						MOV dword ptr [RDI+0x58], 0x00000000   # Blue
						# JMP draw_bar
					draw_bar:
						add dword ptr [rax], 1
					draw_bar_only_no_add:
						pop rsi
						pop rax
						VMOVAPS XMM0,xmmword ptr [RDI + 0x50]
						jmp 0x0140be05
		*/
		// <!-- Draw bar -->
		write_bytes(app_pid, NO_ASLR(0x01402a91), "50");
		write_bytes(app_pid, NO_ASLR(0x01402a92), "56");
		write_bytes(app_pid, NO_ASLR(0x01402a93), "48c7472800000000");
		write_bytes(app_pid, NO_ASLR(0x01402a9b), "c7473000000042");
		write_bytes(app_pid, NO_ASLR(0x01402aa2), "488d354f5f1204");
		write_bytes(app_pid, NO_ASLR(0x01402aa9), "8b4604");
		write_bytes(app_pid, NO_ASLR(0x01402aac), "c5fa2ac0");
		write_bytes(app_pid, NO_ASLR(0x01402ab0), "c5fa114734");
		write_bytes(app_pid, NO_ASLR(0x01402ab5), "488d0584950d04");
		write_bytes(app_pid, NO_ASLR(0x01402abc), "c747500000803f");
		write_bytes(app_pid, NO_ASLR(0x01402ac3), "c747540000803f");
		write_bytes(app_pid, NO_ASLR(0x01402aca), "c747580000803f");
		write_bytes(app_pid, NO_ASLR(0x01402ad1), "c7475c0000803f");
		write_bytes(app_pid, NO_ASLR(0x01402ad8), "83380f");
		write_bytes(app_pid, NO_ASLR(0x01402adb), "0f8790000000");
		write_bytes(app_pid, NO_ASLR(0x01402ae1), "833800");
		write_bytes(app_pid, NO_ASLR(0x01402ae4), "0f8492000000");
		write_bytes(app_pid, NO_ASLR(0x01402aea), "833801");
		write_bytes(app_pid, NO_ASLR(0x01402aed), "0f84a3000000");
		write_bytes(app_pid, NO_ASLR(0x01402af3), "833802");
		write_bytes(app_pid, NO_ASLR(0x01402af6), "0f84b4000000");
		write_bytes(app_pid, NO_ASLR(0x01402afc), "833803");
		write_bytes(app_pid, NO_ASLR(0x01402aff), "0f84c5000000");
		write_bytes(app_pid, NO_ASLR(0x01402b05), "833804");
		write_bytes(app_pid, NO_ASLR(0x01402b08), "0f84d6000000");
		write_bytes(app_pid, NO_ASLR(0x01402b0e), "833805");
		write_bytes(app_pid, NO_ASLR(0x01402b11), "0f84e7000000");
		write_bytes(app_pid, NO_ASLR(0x01402b17), "833806");
		write_bytes(app_pid, NO_ASLR(0x01402b1a), "0f84f8000000");
		write_bytes(app_pid, NO_ASLR(0x01402b20), "833807");
		write_bytes(app_pid, NO_ASLR(0x01402b23), "0f8409010000");
		write_bytes(app_pid, NO_ASLR(0x01402b29), "833808");
		write_bytes(app_pid, NO_ASLR(0x01402b2c), "0f841a010000");
		write_bytes(app_pid, NO_ASLR(0x01402b32), "833809");
		write_bytes(app_pid, NO_ASLR(0x01402b35), "0f842b010000");
		write_bytes(app_pid, NO_ASLR(0x01402b3b), "83380a");
		write_bytes(app_pid, NO_ASLR(0x01402b3e), "0f843c010000");
		write_bytes(app_pid, NO_ASLR(0x01402b44), "83380b");
		write_bytes(app_pid, NO_ASLR(0x01402b47), "0f844a010000");
		write_bytes(app_pid, NO_ASLR(0x01402b4d), "83380c");
		write_bytes(app_pid, NO_ASLR(0x01402b50), "0f8458010000");
		write_bytes(app_pid, NO_ASLR(0x01402b56), "83380d");
		write_bytes(app_pid, NO_ASLR(0x01402b59), "0f8466010000");
		write_bytes(app_pid, NO_ASLR(0x01402b5f), "83380e");
		write_bytes(app_pid, NO_ASLR(0x01402b62), "0f8474010000");
		write_bytes(app_pid, NO_ASLR(0x01402b68), "83380f");
		write_bytes(app_pid, NO_ASLR(0x01402b6b), "0f8482010000");
		write_bytes(app_pid, NO_ASLR(0x01402b71), "c70001000000");
		write_bytes(app_pid, NO_ASLR(0x01402b77), "e98f010000");
		write_bytes(app_pid, NO_ASLR(0x01402b7c), "c747500000803f");
		write_bytes(app_pid, NO_ASLR(0x01402b83), "c747540000803f");
		write_bytes(app_pid, NO_ASLR(0x01402b8a), "c747580000803f");
		write_bytes(app_pid, NO_ASLR(0x01402b91), "e972010000");
		write_bytes(app_pid, NO_ASLR(0x01402b96), "c7475000000000");
		write_bytes(app_pid, NO_ASLR(0x01402b9d), "c747540000803f");
		write_bytes(app_pid, NO_ASLR(0x01402ba4), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402bab), "e958010000");
		write_bytes(app_pid, NO_ASLR(0x01402bb0), "c7475000000000");
		write_bytes(app_pid, NO_ASLR(0x01402bb7), "c7475400000000");
		write_bytes(app_pid, NO_ASLR(0x01402bbe), "c747580000803f");
		write_bytes(app_pid, NO_ASLR(0x01402bc5), "e93e010000");
		write_bytes(app_pid, NO_ASLR(0x01402bca), "c747500000803f");
		write_bytes(app_pid, NO_ASLR(0x01402bd1), "c7475400000000");
		write_bytes(app_pid, NO_ASLR(0x01402bd8), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402bdf), "e924010000");
		write_bytes(app_pid, NO_ASLR(0x01402be4), "c7475000000000");
		write_bytes(app_pid, NO_ASLR(0x01402beb), "c747540000003f");
		write_bytes(app_pid, NO_ASLR(0x01402bf2), "c747580000003f");
		write_bytes(app_pid, NO_ASLR(0x01402bf9), "e90a010000");
		write_bytes(app_pid, NO_ASLR(0x01402bfe), "c7475000000000");
		write_bytes(app_pid, NO_ASLR(0x01402c05), "c7475400000000");
		write_bytes(app_pid, NO_ASLR(0x01402c0c), "c747580000003f");
		write_bytes(app_pid, NO_ASLR(0x01402c13), "e9f0000000");
		write_bytes(app_pid, NO_ASLR(0x01402c18), "c7475000000000");
		write_bytes(app_pid, NO_ASLR(0x01402c1f), "c747540000003f");
		write_bytes(app_pid, NO_ASLR(0x01402c26), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402c2d), "e9d6000000");
		write_bytes(app_pid, NO_ASLR(0x01402c32), "c7475000000000");
		write_bytes(app_pid, NO_ASLR(0x01402c39), "c747540000803f");
		write_bytes(app_pid, NO_ASLR(0x01402c40), "c747580000803f");
		write_bytes(app_pid, NO_ASLR(0x01402c47), "e9bc000000");
		write_bytes(app_pid, NO_ASLR(0x01402c4c), "c747500000003f");
		write_bytes(app_pid, NO_ASLR(0x01402c53), "c7475400000000");
		write_bytes(app_pid, NO_ASLR(0x01402c5a), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402c61), "e9a2000000");
		write_bytes(app_pid, NO_ASLR(0x01402c66), "c747500000403f");
		write_bytes(app_pid, NO_ASLR(0x01402c6d), "c747540000403f");
		write_bytes(app_pid, NO_ASLR(0x01402c74), "c747580000403f");
		write_bytes(app_pid, NO_ASLR(0x01402c7b), "e988000000");
		write_bytes(app_pid, NO_ASLR(0x01402c80), "c747500000003f");
		write_bytes(app_pid, NO_ASLR(0x01402c87), "c7475400000000");
		write_bytes(app_pid, NO_ASLR(0x01402c8e), "c747580000003f");
		write_bytes(app_pid, NO_ASLR(0x01402c95), "eb71");
		write_bytes(app_pid, NO_ASLR(0x01402c97), "c747500000003f");
		write_bytes(app_pid, NO_ASLR(0x01402c9e), "c747540000003f");
		write_bytes(app_pid, NO_ASLR(0x01402ca5), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402cac), "eb5a");
		write_bytes(app_pid, NO_ASLR(0x01402cae), "c747500000003f");
		write_bytes(app_pid, NO_ASLR(0x01402cb5), "c747540000003f");
		write_bytes(app_pid, NO_ASLR(0x01402cbc), "c747580000003f");
		write_bytes(app_pid, NO_ASLR(0x01402cc3), "eb43");
		write_bytes(app_pid, NO_ASLR(0x01402cc5), "c747500000803f");
		write_bytes(app_pid, NO_ASLR(0x01402ccc), "c7475400000000");
		write_bytes(app_pid, NO_ASLR(0x01402cd3), "c747580000803f");
		write_bytes(app_pid, NO_ASLR(0x01402cda), "eb2c");
		write_bytes(app_pid, NO_ASLR(0x01402cdc), "c747500000803f");
		write_bytes(app_pid, NO_ASLR(0x01402ce3), "c747540000803f");
		write_bytes(app_pid, NO_ASLR(0x01402cea), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402cf1), "eb15");
		write_bytes(app_pid, NO_ASLR(0x01402cf3), "c747500000803f");
		write_bytes(app_pid, NO_ASLR(0x01402cfa), "c747540000003f");
		write_bytes(app_pid, NO_ASLR(0x01402d01), "c7475800000000");
		write_bytes(app_pid, NO_ASLR(0x01402d08), "830001");
		write_bytes(app_pid, NO_ASLR(0x01402d0b), "5e");
		write_bytes(app_pid, NO_ASLR(0x01402d0c), "58");
		write_bytes(app_pid, NO_ASLR(0x01402d0d), "c5f8284750");
		write_bytes(app_pid, NO_ASLR(0x01402d12), "e9ee900000");
		printf_notification("Bloodborne: Color Border Patched");
	}
	// 120Hz
	if (g_isPatch120Hz)
	{
		/*
		LEA RAX,[0x057e4ee8]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x5610
		MOV dword ptr [0x054dc008],EDI
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,0x1
		JNZ 0x0081a6cd
		LEA RAX,[0x057e4ee8]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x1840
		MOV EDI,dword ptr [0x054dc008]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		JMP 0x02ad61f3
		*/
		write_bytes(app_pid, NO_ASLR(0x0243487e), "41c74424186f12833a");
		write_bytes(app_pid, NO_ASLR(0x02ad61ee), "e98d44d4fd");
		write_bytes(app_pid, NO_ASLR(0x0081a680), "488d0561a8fc04");
		write_bytes(app_pid, NO_ASLR(0x0081a687), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0081a68a), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x0081a690), "893d7219cc04");
		write_bytes(app_pid, NO_ASLR(0x0081a696), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x0081a69b), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0081a69d), "31c9");
		write_bytes(app_pid, NO_ASLR(0x0081a69f), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x0081a6a2), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x0081a6a4), "83f801");
		write_bytes(app_pid, NO_ASLR(0x0081a6a7), "7524");
		write_bytes(app_pid, NO_ASLR(0x0081a6a9), "488d0538a8fc04");
		write_bytes(app_pid, NO_ASLR(0x0081a6b0), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0081a6b3), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x0081a6b9), "8b3d4919cc04");
		write_bytes(app_pid, NO_ASLR(0x0081a6bf), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x0081a6c4), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0081a6c6), "31c9");
		write_bytes(app_pid, NO_ASLR(0x0081a6c8), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x0081a6cb), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x0081a6cd), "e921bb2b02");
		printf_notification("Bloodborne: Applied 120Hz Patch");
	}
	/*
	else
	{
		write_bytes(app_pid, NO_ASLR(0x020367c7), "488d351cd2c702");
		write_bytes(app_pid, NO_ASLR(0x020369e8), "488d35b5cfc702");
		write_wstring(app_pid, NO_ASLR(0x04cb39ea), L"This 60fps patch was");
		write_wstring(app_pid, NO_ASLR(0x04cb39a4), L"Made with love, by Lance McDonald");
		write_bytes(app_pid, NO_ASLR(0x0243487e), "41c74424188988883c");
		write_float32(app_pid, NO_ASLR(0x00a3fc3f), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x00f383ca), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x012b5458), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x012b55b6), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x013734f3), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x016efe5c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01972a80), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01bf9b74), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01c0ca9a), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01daa65e), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea4003), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea40a0), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea4146), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea49ce), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea4bda), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea5248), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea5476), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea552d), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea57c0), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea5ac5), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea6015), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea6045), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea6275), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea62a5), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ea632f), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01eb540b), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01eb54dc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01eb90aa), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01eb9117), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ebb111), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ebf0d3), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec3f69), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec3f9a), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec3fd8), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec4009), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec404e), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec40a4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec40e0), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec4136), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec4167), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec41a5), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01ec41dc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01f032f2), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01f03762), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01f6aa32), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x01f96d9a), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0200d8e2), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0200e142), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0200e2d4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0200fa52), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x020119d2), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02012652), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x020127cc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02012932), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02012ac4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02012c32), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0222bcd7), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02418e39), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02418f38), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0243460c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x0243485f), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02434883), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x024348b8), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02fda86c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02fdc3d4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x02fdf7e4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04befdcc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04c88688), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cad65c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cad660), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cad664), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cadc08), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cadd28), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04caf1bc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04caf1c0), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04caf290), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04caf294), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04caf340), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04caf344), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cb03b8), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04cb03bc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04ccc3f4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d2577c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d25844), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d2585c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d25864), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d25e94), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d26364), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d265fc), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d26618), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d26c9c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d270e4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d27250), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d27258), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d27284), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d2728c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d27554), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d275f4), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d27720), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d27c4c), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d28418), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d28e98), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d29124), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d29128), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d29170), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d29b80), (1.f / 60.f));
		write_float32(app_pid, NO_ASLR(0x04d29e74), (1.f / 60.f));
	}
	*/
}

void DoPatch_GravityDaze2_111(pid_t app_pid, uint64_t text_base)
{
	if (parseXML(GR2_60HzKey))
	{
		// write_bytes(app_pid, NO_ASLR(0x00901c26), "c5fa10058a68fb00");
		// write_bytes(app_pid, NO_ASLR(0x00907e20), "c3");
		// <!-- Boot game in unused 60FPS Mode -->
		write_bytes(app_pid, NO_ASLR(0x00854ac5), "c605bc656a0100");
		/*
		write_bytes(app_pid, NO_ASLR(0x00854acc), "c705ae656a0100007042");
		write_bytes(app_pid, NO_ASLR(0x00854ad6), "c705a0656a018988883c");
		write_bytes(app_pid, NO_ASLR(0x00854b12), "c705f4656a018988883c");
		write_bytes(app_pid, NO_ASLR(0x00854b1c), "c7050a666a018988883c");
		write_bytes(app_pid, NO_ASLR(0x00854b26), "c70520666a0100007042");
		*/
		// <!-- half rate EnemyBrain:getPlayMotionFrame -->
		// <!-- for 120hz, maybe keep this? -->
		write_bytes32(app_pid, NO_ASLR(0x018c7374), 0x41f00000);
		// <!-- No fliprate -->
		write_bytes(app_pid, NO_ASLR(0x00846980), "be00000000");
		// <!-- No wait in render thread (up to 240fps) -->
		write_bytes(app_pid, NO_ASLR(0x0084dda4), "bf46100000");
		write_bytes(app_pid, NO_ASLR(0x0084dda9), "eb08");
		// <!-- branch to delta time to timestep func -->
		write_bytes(app_pid, NO_ASLR(0x0085e9cc), "e8cd43fa00");
		// <!-- delta time to timestep -->
		// <!-- const -->
		write_bytes32(app_pid, NO_ASLR(0x01802d90), 0x49742400);
		// <!-- 15fps limit -->
		write_bytes64(app_pid, NO_ASLR(0x01802d94), 0x000000000001046b);
		// <!-- delta time to timestep func -->
		write_bytes(app_pid, NO_ASLR(0x01802d9e), "4889150b533000");
		write_bytes(app_pid, NO_ASLR(0x01802da5), "48893d0c533000");
		write_bytes(app_pid, NO_ASLR(0x01802dac), "488b3de1ffffff");
		write_bytes(app_pid, NO_ASLR(0x01802db3), "488d15b6523000");
		write_bytes(app_pid, NO_ASLR(0x01802dba), "48393d07587500");
		write_bytes(app_pid, NO_ASLR(0x01802dc1), "771c");
		write_bytes(app_pid, NO_ASLR(0x01802dc3), "803a01");
		write_bytes(app_pid, NO_ASLR(0x01802dc6), "7417");
		write_bytes(app_pid, NO_ASLR(0x01802dc8), "803a03");
		write_bytes(app_pid, NO_ASLR(0x01802dcb), "7412");
		write_bytes(app_pid, NO_ASLR(0x01802dcd), "c5222a1df3577500");
		write_bytes(app_pid, NO_ASLR(0x01802dd5), "c5a25e0db3ffffff");
		write_bytes(app_pid, NO_ASLR(0x01802ddd), "eb08");
		write_bytes(app_pid, NO_ASLR(0x01802ddf), "c5fa100d3d836f00");
		write_bytes(app_pid, NO_ASLR(0x01802de7), "488b15c2523000");
		write_bytes(app_pid, NO_ASLR(0x01802dee), "488b3dc3523000");
		write_bytes(app_pid, NO_ASLR(0x01802df5), "c3");
		// <!-- menu fixes (incomplete) -->
		write_bytes32(app_pid, NO_ASLR(0x018c4934), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x013e46e8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c491c), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c4964), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3aa0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3aa4), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3aa8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3aac), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ab0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ab4), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ab8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3abc), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ac0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ac8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ad0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ad8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3adc), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ae0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3ae4), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3afc), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b04), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b08), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b0c), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b10), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b14), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b18), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b3c), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b40), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b50), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3b60), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3bc4), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3bc8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3bd0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3be8), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3bec), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3bf0), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3c08), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3c14), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3c18), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3c20), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f1c), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f20), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f38), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f40), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f44), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f5c), 0x3c888889);
		write_bytes32(app_pid, NO_ASLR(0x018c3f60), 0x3c888889);
		write_bytes(app_pid, NO_ASLR(0x013c8f2f), "e8479e4300");
		write_bytes(app_pid, NO_ASLR(0x013c8f34), "eb1b");
		write_bytes(app_pid, NO_ASLR(0x01802d7b), "c605ee52300003");
		write_bytes(app_pid, NO_ASLR(0x01802d82), "c3");
		write_bytes(app_pid, NO_ASLR(0x013c8f8e), "ebca");
		write_bytes(app_pid, NO_ASLR(0x013c8f5a), "e9249e4300");
		write_bytes(app_pid, NO_ASLR(0x01802d83), "c605e652300002");
		write_bytes(app_pid, NO_ASLR(0x01802d8a), "c3");
		/*
		write_bytes(app_pid, NO_ASLR(0x01627921), "e81cb41d00");
		write_bytes(app_pid, NO_ASLR(0x01802d42), "43c6043c00");
		write_bytes(app_pid, NO_ASLR(0x01802d47), "e929020000");
		write_bytes(app_pid, NO_ASLR(0x01802d54), "488b0db5ffffff");
		write_bytes(app_pid, NO_ASLR(0x01802d5b), "493b0c24");
		write_bytes(app_pid, NO_ASLR(0x01802d5f), "740b");
		write_bytes(app_pid, NO_ASLR(0x01802d61), "498b0c24");
		write_bytes(app_pid, NO_ASLR(0x01802d65), "48890d08533000");
		write_bytes(app_pid, NO_ASLR(0x01802d6c), "c3");
		write_bytes(app_pid, NO_ASLR(0x01802f75), "488d0d4f000000");
		write_bytes(app_pid, NO_ASLR(0x01802f7c), "6641813c24736d");
		write_bytes(app_pid, NO_ASLR(0x01802f83), "7446");
		write_bytes(app_pid, NO_ASLR(0x01802f85), "6641817c24006674");
		write_bytes(app_pid, NO_ASLR(0x01802f8d), "743c");
		write_bytes(app_pid, NO_ASLR(0x01802f8f), "41813c246d696e65");
		write_bytes(app_pid, NO_ASLR(0x01802f97), "7432");
		write_bytes(app_pid, NO_ASLR(0x01802f99), "6641817c24006570");
		write_bytes(app_pid, NO_ASLR(0x01802fa1), "7428");
		write_bytes(app_pid, NO_ASLR(0x01802fa3), "6641817c2400646d");
		write_bytes(app_pid, NO_ASLR(0x01802fab), "741e");
		write_bytes(app_pid, NO_ASLR(0x01802fad), "6641817c2400636d");
		write_bytes(app_pid, NO_ASLR(0x01802fb5), "7414");
		write_bytes(app_pid, NO_ASLR(0x01802fb7), "eb0a");
		write_bytes(app_pid, NO_ASLR(0x01802fc3), "c3");
		write_bytes(app_pid, NO_ASLR(0x01802fcb), "e984fdffff");
		write_bytes(app_pid, NO_ASLR(0x01464e4a), "48e8e4e13900");
		write_bytes(app_pid, NO_ASLR(0x01803034), "ebb6");
		write_bytes(app_pid, NO_ASLR(0x01802fec), "4889059d503000");
		write_bytes(app_pid, NO_ASLR(0x01802ff3), "48891d9e503000");
		write_bytes(app_pid, NO_ASLR(0x01802ffa), "48893d9f503000");
		write_bytes(app_pid, NO_ASLR(0x01803001), "488915a0503000");
		write_bytes(app_pid, NO_ASLR(0x01803008), "eb2c");
		write_bytes(app_pid, NO_ASLR(0x01803036), "488d0533503000");
		write_bytes(app_pid, NO_ASLR(0x0180303d), "488d1de1000000");
		write_bytes(app_pid, NO_ASLR(0x01803044), "c6400b00");
		write_bytes(app_pid, NO_ASLR(0x01803048), "eb06");
		write_bytes(app_pid, NO_ASLR(0x01803050), "48894814");
		write_bytes(app_pid, NO_ASLR(0x01803054), "488b4804");
		write_bytes(app_pid, NO_ASLR(0x01803058), "e8eb8e0000");
		write_bytes(app_pid, NO_ASLR(0x0180305d), "e8b1fd0200");
		write_bytes(app_pid, NO_ASLR(0x01832e13), "e8720c0000");
		write_bytes(app_pid, NO_ASLR(0x01832e18), "4c8d0d59a76e00");
		write_bytes(app_pid, NO_ASLR(0x01832e1f), "4c8d154a522d00");
		write_bytes(app_pid, NO_ASLR(0x01832e26), "41bb04010000");
		write_bytes(app_pid, NO_ASLR(0x01832e2c), "45391c21");
		write_bytes(app_pid, NO_ASLR(0x01832e30), "7536");
		write_bytes(app_pid, NO_ASLR(0x01832e32), "45395c2254");
		write_bytes(app_pid, NO_ASLR(0x01832e37), "743e");
		write_bytes(app_pid, NO_ASLR(0x01832e39), "41807a5a01");
		write_bytes(app_pid, NO_ASLR(0x01832e3e), "7413");
		write_bytes(app_pid, NO_ASLR(0x01832e40), "41807a5a02");
		write_bytes(app_pid, NO_ASLR(0x01832e45), "7411");
		write_bytes(app_pid, NO_ASLR(0x01832e47), "41807a5a03");
		write_bytes(app_pid, NO_ASLR(0x01832e4c), "750d");
		write_bytes(app_pid, NO_ASLR(0x01832e4e), "41b001");
		write_bytes(app_pid, NO_ASLR(0x01832e51), "eb08");
		write_bytes(app_pid, NO_ASLR(0x01832e53), "41b002");
		write_bytes(app_pid, NO_ASLR(0x01832e56), "eb03");
		write_bytes(app_pid, NO_ASLR(0x01832e58), "41b001");
		write_bytes(app_pid, NO_ASLR(0x01832e5b), "4588425a");
		write_bytes(app_pid, NO_ASLR(0x01832e5f), "45884200");
		write_bytes(app_pid, NO_ASLR(0x01832e63), "4589d8");
		write_bytes(app_pid, NO_ASLR(0x01832e66), "eb03");
		write_bytes(app_pid, NO_ASLR(0x01832e68), "458b01");
		write_bytes(app_pid, NO_ASLR(0x01832e6b), "45894254");
		write_bytes(app_pid, NO_ASLR(0x01832e6f), "458a4200");
		write_bytes(app_pid, NO_ASLR(0x01832e73), "4588425a");
		write_bytes(app_pid, NO_ASLR(0x01832e77), "e82b0c0000");
		write_bytes(app_pid, NO_ASLR(0x01832e7c), "c3");
		write_bytes(app_pid, NO_ASLR(0x01833a8a), "4c89056f462d00");
		write_bytes(app_pid, NO_ASLR(0x01833a91), "4c890d70462d00");
		write_bytes(app_pid, NO_ASLR(0x01833a98), "4c891571462d00");
		write_bytes(app_pid, NO_ASLR(0x01833a9f), "4c891d72462d00");
		write_bytes(app_pid, NO_ASLR(0x01833aa6), "c3");
		write_bytes(app_pid, NO_ASLR(0x01833aa7), "4c8b0552462d00");
		write_bytes(app_pid, NO_ASLR(0x01833aae), "4c8b0d53462d00");
		write_bytes(app_pid, NO_ASLR(0x01833ab5), "4c8b1554462d00");
		write_bytes(app_pid, NO_ASLR(0x01833abc), "4c8b1d55462d00");
		write_bytes(app_pid, NO_ASLR(0x01833ac3), "c3");
		write_bytes(app_pid, NO_ASLR(0x0180bf48), "48390b");
		write_bytes(app_pid, NO_ASLR(0x0180bf4b), "0f843a120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf51), "48394b08");
		write_bytes(app_pid, NO_ASLR(0x0180bf55), "0f8430120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf5b), "48394b10");
		write_bytes(app_pid, NO_ASLR(0x0180bf5f), "0f8426120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf65), "394b18");
		write_bytes(app_pid, NO_ASLR(0x0180bf68), "0f841d120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf6e), "394b1d");
		write_bytes(app_pid, NO_ASLR(0x0180bf71), "0f8414120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf77), "48394b22");
		write_bytes(app_pid, NO_ASLR(0x0180bf7b), "0f840a120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf81), "48394b2a");
		write_bytes(app_pid, NO_ASLR(0x0180bf85), "0f8400120000");
		write_bytes(app_pid, NO_ASLR(0x0180bf8b), "48394b32");
		write_bytes(app_pid, NO_ASLR(0x0180bf8f), "0f84f6110000");
		write_bytes(app_pid, NO_ASLR(0x0180bf95), "48394b3a");
		write_bytes(app_pid, NO_ASLR(0x0180bf99), "0f84ec110000");
		write_bytes(app_pid, NO_ASLR(0x0180bf9f), "48394b42");
		write_bytes(app_pid, NO_ASLR(0x0180bfa3), "0f84e2110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfa9), "48394b4a");
		write_bytes(app_pid, NO_ASLR(0x0180bfad), "0f84d8110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfb3), "48394b52");
		write_bytes(app_pid, NO_ASLR(0x0180bfb7), "0f84ce110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfbd), "48394b5a");
		write_bytes(app_pid, NO_ASLR(0x0180bfc1), "0f84c4110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfc7), "48394b62");
		write_bytes(app_pid, NO_ASLR(0x0180bfcb), "0f84ba110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfd1), "48394b6a");
		write_bytes(app_pid, NO_ASLR(0x0180bfd5), "0f84b0110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfdb), "48394b72");
		write_bytes(app_pid, NO_ASLR(0x0180bfdf), "0f84a6110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfe5), "48398b88000000");
		write_bytes(app_pid, NO_ASLR(0x0180bfec), "0f8499110000");
		write_bytes(app_pid, NO_ASLR(0x0180bff2), "48398b90000000");
		write_bytes(app_pid, NO_ASLR(0x0180bff9), "0f848c110000");
		write_bytes(app_pid, NO_ASLR(0x0180bfff), "398b98000000");
		write_bytes(app_pid, NO_ASLR(0x0180c005), "0f8480110000");
		write_bytes(app_pid, NO_ASLR(0x0180c00b), "48398b9d000000");
		write_bytes(app_pid, NO_ASLR(0x0180c012), "0f8473110000");
		write_bytes(app_pid, NO_ASLR(0x0180c018), "398c23a5000000");
		write_bytes(app_pid, NO_ASLR(0x0180c01f), "0f8466110000");
		write_bytes(app_pid, NO_ASLR(0x0180c025), "48398baf000000");
		write_bytes(app_pid, NO_ASLR(0x0180c02c), "0f8459110000");
		write_bytes(app_pid, NO_ASLR(0x0180c032), "e957110000");
		write_bytes(app_pid, NO_ASLR(0x0180d18b), "c60001");
		write_bytes(app_pid, NO_ASLR(0x0180d18e), "c3");
		write_bytes(app_pid, NO_ASLR(0x01803067), "e984000000");
		write_bytes(app_pid, NO_ASLR(0x018030f0), "bf0001104e");
		write_bytes(app_pid, NO_ASLR(0x018030f5), "803801");
		write_bytes(app_pid, NO_ASLR(0x018030f8), "740c");
		write_bytes(app_pid, NO_ASLR(0x018030fa), "be00000000");
		write_bytes(app_pid, NO_ASLR(0x018030ff), "e80c49ffff");
		write_bytes(app_pid, NO_ASLR(0x01803104), "eb0a");
		write_bytes(app_pid, NO_ASLR(0x01803106), "be01000000");
		write_bytes(app_pid, NO_ASLR(0x0180310b), "e80049ffff");
		write_bytes(app_pid, NO_ASLR(0x01803110), "e8f5feffff");
		write_bytes(app_pid, NO_ASLR(0x01803115), "4889fb");
		write_bytes(app_pid, NO_ASLR(0x01803118), "488b03");
		write_bytes(app_pid, NO_ASLR(0x0180311b), "c3");
		write_bytes(app_pid, NO_ASLR(0x0180300a), "488b057f503000");
		write_bytes(app_pid, NO_ASLR(0x01803011), "488b1d80503000");
		write_bytes(app_pid, NO_ASLR(0x01803018), "488b3d81503000");
		write_bytes(app_pid, NO_ASLR(0x0180301f), "488b1582503000");
		write_bytes(app_pid, NO_ASLR(0x01803026), "c3");
		write_bytes(app_pid, NO_ASLR(0x015111cf), "e8d0a02e00");
		write_bytes(app_pid, NO_ASLR(0x015111d4), "90");
		write_bytes(app_pid, NO_ASLR(0x015111d5), "90");
		write_bytes(app_pid, NO_ASLR(0x015111d6), "90");
		write_bytes(app_pid, NO_ASLR(0x015111d7), "90");
		write_bytes(app_pid, NO_ASLR(0x015111d8), "90");
		write_bytes(app_pid, NO_ASLR(0x015111d9), "90");
		write_bytes(app_pid, NO_ASLR(0x015111da), "90");
		write_bytes(app_pid, NO_ASLR(0x015111db), "90");
		write_bytes(app_pid, NO_ASLR(0x015111dc), "90");
		write_bytes(app_pid, NO_ASLR(0x017fb2a4), "803d31ce300001");
		write_bytes(app_pid, NO_ASLR(0x017fb2ab), "7410");
		write_bytes(app_pid, NO_ASLR(0x017fb2ad), "41803c2400");
		write_bytes(app_pid, NO_ASLR(0x017fb2b2), "7510");
		write_bytes(app_pid, NO_ASLR(0x017fb2b4), "4c8d25ab000000");
		write_bytes(app_pid, NO_ASLR(0x017fb2bb), "eb07");
		write_bytes(app_pid, NO_ASLR(0x017fb2bd), "4c8d25b0cd3000");
		write_bytes(app_pid, NO_ASLR(0x017fb2c4), "c3");
		write_bytes(app_pid, NO_ASLR(0x01803062), "e862fe0200");
		write_bytes(app_pid, NO_ASLR(0x01832ec9), "e8bc0b0000");
		write_bytes(app_pid, NO_ASLR(0x01832ece), "4c8d0da3a66e00");
		write_bytes(app_pid, NO_ASLR(0x01832ed5), "4989c2");
		write_bytes(app_pid, NO_ASLR(0x01832ed8), "41bb08020000");
		write_bytes(app_pid, NO_ASLR(0x01832ede), "45391c21");
		write_bytes(app_pid, NO_ASLR(0x01832ee2), "7523");
		write_bytes(app_pid, NO_ASLR(0x01832ee4), "45395c2260");
		write_bytes(app_pid, NO_ASLR(0x01832ee9), "742b");
		write_bytes(app_pid, NO_ASLR(0x01832eeb), "41807a6400");
		write_bytes(app_pid, NO_ASLR(0x01832ef0), "7505");
		write_bytes(app_pid, NO_ASLR(0x01832ef2), "41b001");
		write_bytes(app_pid, NO_ASLR(0x01832ef5), "eb03");
		write_bytes(app_pid, NO_ASLR(0x01832ef7), "41b000");
		write_bytes(app_pid, NO_ASLR(0x01832efa), "45884264");
		write_bytes(app_pid, NO_ASLR(0x01832efe), "4588426c");
		write_bytes(app_pid, NO_ASLR(0x01832f02), "4589d8");
		write_bytes(app_pid, NO_ASLR(0x01832f05), "eb03");
		write_bytes(app_pid, NO_ASLR(0x01832f07), "458b01");
		write_bytes(app_pid, NO_ASLR(0x01832f0a), "45894260");
		write_bytes(app_pid, NO_ASLR(0x01832f0e), "458a426c");
		write_bytes(app_pid, NO_ASLR(0x01832f12), "45884264");
		write_bytes(app_pid, NO_ASLR(0x01832f16), "e88c0b0000");
		write_bytes(app_pid, NO_ASLR(0x01832f1b), "c3");
		*/
		// Force Enable Raven DLC
		write_bytes(app_pid, NO_ASLR(0x00430a63), "488d3d15000000");
		write_bytes(app_pid, NO_ASLR(0x00430a6a), "31f6");
		write_bytes(app_pid, NO_ASLR(0x00430a6c), "31d2");
		write_bytes(app_pid, NO_ASLR(0x00430a6e), "e8bd5b4300");
		write_bytes(app_pid, NO_ASLR(0x00430a73), "0fb605a9ee8a01");
		write_bytes(app_pid, NO_ASLR(0x00430a7a), "e9bf554300");
		write_bytes(app_pid, NO_ASLR(0x00430a7f), "43524f5700");
		write_bytes(app_pid, NO_ASLR(0x00866037), "e927aabcff");
		// <!-- dm06_03000 check -->
		write_bytes(app_pid, NO_ASLR(0x011bb6cc), "e9647a6400");
		write_bytes(app_pid, NO_ASLR(0x01802d10), "646d30365f303330303000");
		write_bytes(app_pid, NO_ASLR(0x01803135), "488d3dd4fbffff");
		write_bytes(app_pid, NO_ASLR(0x0180313c), "4889de");
		write_bytes(app_pid, NO_ASLR(0x0180313f), "e8dc25ffff");
		write_bytes(app_pid, NO_ASLR(0x01803144), "83f800");
		write_bytes(app_pid, NO_ASLR(0x01803147), "7507");
		write_bytes(app_pid, NO_ASLR(0x01803149), "c605204f300003");
		write_bytes(app_pid, NO_ASLR(0x01803150), "4889df");
		write_bytes(app_pid, NO_ASLR(0x01803153), "e82825ffff");
		write_bytes(app_pid, NO_ASLR(0x01803158), "4989c4");
		write_bytes(app_pid, NO_ASLR(0x0180315b), "e977859bff");
		/*
		LEA RDI,[0x1f7b668]
		MOV dword ptr [RDI],EAX
		LEA RAX,[0x01b05e68]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x5610
		MOV EDI,dword ptr [0x01f7b668]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,0x1
		JNZ exit
		LEA RAX,[0x01b05e68]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x1840
		MOV EDI,dword ptr [0x01f7b668]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		exit:
		JMP 0x00846813
		*/
		printf_notification("Applied 60 FPS");
	}
	// 120Hz
	if (g_isPatch120Hz)
	{
		write_bytes(app_pid, NO_ASLR(0x0050bc03), "488d3d5efaa601");
		write_bytes(app_pid, NO_ASLR(0x0050bc0a), "8907");
		write_bytes(app_pid, NO_ASLR(0x0050bc0c), "488d0555a25f01");
		write_bytes(app_pid, NO_ASLR(0x0050bc13), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0050bc16), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x0050bc1c), "8b3d46faa601");
		write_bytes(app_pid, NO_ASLR(0x0050bc22), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x0050bc27), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0050bc29), "31c9");
		write_bytes(app_pid, NO_ASLR(0x0050bc2b), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x0050bc2e), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x0050bc30), "83f801");
		write_bytes(app_pid, NO_ASLR(0x0050bc33), "7524");
		write_bytes(app_pid, NO_ASLR(0x0050bc35), "488d052ca25f01");
		write_bytes(app_pid, NO_ASLR(0x0050bc3c), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0050bc3f), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x0050bc45), "8b3d1dfaa601");
		write_bytes(app_pid, NO_ASLR(0x0050bc4b), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x0050bc50), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0050bc52), "31c9");
		write_bytes(app_pid, NO_ASLR(0x0050bc54), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x0050bc57), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x0050bc59), "e9b5ab3300");
		write_bytes(app_pid, NO_ASLR(0x0084680d), "e9f153ccff");
		printf_notification("Applied 120Hz Patch");
	}
}

void DoPatch_SOTC_100(pid_t app_pid, uint64_t text_base)
{
	if (parseXML(SOTC_DebugMenu))
	{
		write_bytes(app_pid, NO_ASLR(0x0042437f), "0f1f440000");
		printf_notification("Applied Debug Menu");
	}
	if (g_isPatch120Hz)
	{
		// write_bytes(app_pid, NO_ASLR(0x01390090), "31c0c3");
		write_bytes(app_pid, NO_ASLR(0x013900a0), "31c0c3");
		write_bytes(app_pid, NO_ASLR(0x004959a4), "e9d7de4f01");
		write_bytes(app_pid, NO_ASLR(0x01993880), "e85bc79fff");
		write_bytes(app_pid, NO_ASLR(0x01993885), "488d0594cd0600");
		write_bytes(app_pid, NO_ASLR(0x0199388c), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0199388f), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x01993895), "8b7b68");
		write_bytes(app_pid, NO_ASLR(0x01993898), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x0199389d), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0199389f), "31c9");
		write_bytes(app_pid, NO_ASLR(0x019938a1), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x019938a4), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x019938a6), "83f801");
		write_bytes(app_pid, NO_ASLR(0x019938a9), "7521");
		write_bytes(app_pid, NO_ASLR(0x019938ab), "488d056ecd0600");
		write_bytes(app_pid, NO_ASLR(0x019938b2), "488b00");
		write_bytes(app_pid, NO_ASLR(0x019938b5), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x019938bb), "8b7b68");
		write_bytes(app_pid, NO_ASLR(0x019938be), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x019938c3), "31d2");
		write_bytes(app_pid, NO_ASLR(0x019938c5), "31c9");
		write_bytes(app_pid, NO_ASLR(0x019938c7), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x019938ca), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x019938cc), "e9d820b0fe");
		printf_notification("Applied 120Hz Patch");
	}
}

void DoPatch_BigCollection(pid_t app_pid, uint64_t text_base, uint32_t idx)
{
	switch (idx)
	{
	case (0x100 << 1):
	{
		write_bytes(app_pid, NO_ASLR(0x00d98997), "01");
		write_bytes(app_pid, NO_ASLR(0x008a0d42), "41c686ba39000000");
		write_bytes(app_pid, NO_ASLR(0x006d00ae), "909090909090");
		write_bytes(app_pid, NO_ASLR(0x004de0db), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004de11c), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004de19b), "e9f80c0000");
		write_bytes(app_pid, NO_ASLR(0x004dee98), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df5b1), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x0066df04), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x0066e158), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df732), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df793), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df7f4), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df855), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df956), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004df9b7), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfa50), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfa96), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfaa8), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfaba), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfb1b), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfb7c), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfbb5), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfe10), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004dfe73), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004e002a), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x004fe7b4), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x00500b3e), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x006a3dff), "b801000000");
		printf_notification("Applied Debug Menu");
		// 120hz
		/*
		LEA RAX,[0xd874a0]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x5610
		MOV EDI,dword ptr [0x013188b0]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,0x1
		JNZ 0x00b88f89
		LEA RAX,[0xd874a0]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x1840
		MOV EDI,dword ptr [0x013188b0]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		XOR EDI,EDI
		CALL 0x00763400
		XOR EAX,EAX
		POP RBP
		RET
		*/
		// 120Hz
		// if (g_isPatch120Hz)
		{
			write_bytes(app_pid, NO_ASLR(0x007fbdc9), "e96ed13800");
			write_bytes(app_pid, NO_ASLR(0x00b88f3c), "488d055de51f00");
			write_bytes(app_pid, NO_ASLR(0x00b88f43), "488b00");
			write_bytes(app_pid, NO_ASLR(0x00b88f46), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x00b88f4c), "8b3d5ef97800");
			write_bytes(app_pid, NO_ASLR(0x00b88f52), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x00b88f57), "31d2");
			write_bytes(app_pid, NO_ASLR(0x00b88f59), "31c9");
			write_bytes(app_pid, NO_ASLR(0x00b88f5b), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x00b88f5e), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x00b88f60), "83f801");
			write_bytes(app_pid, NO_ASLR(0x00b88f63), "7524");
			write_bytes(app_pid, NO_ASLR(0x00b88f65), "488d0534e51f00");
			write_bytes(app_pid, NO_ASLR(0x00b88f6c), "488b00");
			write_bytes(app_pid, NO_ASLR(0x00b88f6f), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x00b88f75), "8b3d35f97800");
			write_bytes(app_pid, NO_ASLR(0x00b88f7b), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x00b88f80), "31d2");
			write_bytes(app_pid, NO_ASLR(0x00b88f82), "31c9");
			write_bytes(app_pid, NO_ASLR(0x00b88f84), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x00b88f87), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x00b88f89), "31ff");
			write_bytes(app_pid, NO_ASLR(0x00b88f8b), "e870a4bdff");
			write_bytes(app_pid, NO_ASLR(0x00b88f90), "31c0");
			write_bytes(app_pid, NO_ASLR(0x00b88f92), "5d");
			write_bytes(app_pid, NO_ASLR(0x00b88f93), "c3");
			// double speed video fix
			write_bytes(app_pid, NO_ASLR(0x007a7e9f), "e983173e00");
			write_bytes(app_pid, NO_ASLR(0x007a84d8), "e971113e00");
			write_bytes(app_pid, NO_ASLR(0x00b89627), "488d3d82f27800");
			write_bytes(app_pid, NO_ASLR(0x00b8962e), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x00b89630), "488d0519245d00");
			write_bytes(app_pid, NO_ASLR(0x00b89637), "be01000000");
			write_bytes(app_pid, NO_ASLR(0x00b8963c), "e8af8afeff");
			write_bytes(app_pid, NO_ASLR(0x00b89641), "4c89ef");
			write_bytes(app_pid, NO_ASLR(0x00b89644), "e807d6c1ff");
			write_bytes(app_pid, NO_ASLR(0x00b89649), "e956e8c1ff");
			write_bytes(app_pid, NO_ASLR(0x00b8964e), "488d3d5bf27800");
			write_bytes(app_pid, NO_ASLR(0x00b89655), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x00b89657), "488d05f2235d00");
			write_bytes(app_pid, NO_ASLR(0x00b8965e), "83782c00");
			write_bytes(app_pid, NO_ASLR(0x00b89662), "0f95c0");
			write_bytes(app_pid, NO_ASLR(0x00b89665), "0fb6f0");
			write_bytes(app_pid, NO_ASLR(0x00b89668), "e8838afeff");
			write_bytes(app_pid, NO_ASLR(0x00b8966d), "498b0424");
			write_bytes(app_pid, NO_ASLR(0x00b89671), "483b45d0");
			write_bytes(app_pid, NO_ASLR(0x00b89675), "0f85b5f0c1ff");
			write_bytes(app_pid, NO_ASLR(0x00b8967b), "4889df");
			write_bytes(app_pid, NO_ASLR(0x00b8967e), "4881c418010000");
			write_bytes(app_pid, NO_ASLR(0x00b89685), "5b");
			write_bytes(app_pid, NO_ASLR(0x00b89686), "415c");
			write_bytes(app_pid, NO_ASLR(0x00b89688), "415d");
			write_bytes(app_pid, NO_ASLR(0x00b8968a), "415e");
			write_bytes(app_pid, NO_ASLR(0x00b8968c), "415f");
			write_bytes(app_pid, NO_ASLR(0x00b8968e), "5d");
			write_bytes(app_pid, NO_ASLR(0x00b8968f), "e9dcccc1ff");
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	case (0x100 << 2):
	{
		write_bytes(app_pid, NO_ASLR(0x00a373aa), "41c686ba39000000");
		write_bytes(app_pid, NO_ASLR(0x005c06bf), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c06ff), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x00941c93), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c0723), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c0c60), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x007760d2), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x007760f6), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x007762a6), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x007762e3), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x00776317), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x00777699), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c0d3d), "e92d000000");
		write_bytes(app_pid, NO_ASLR(0x005c0dc6), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c0dcb), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x00638db2), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x00638dd6), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x0063bf08), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x0063c1e8), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x0063c20c), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x0063c4bd), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x00824d3a), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x00825840), "c3");
		write_bytes(app_pid, NO_ASLR(0x00825840), "c3");
		write_bytes(app_pid, NO_ASLR(0x0082580a), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c0e2b), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c0e4b), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c0ea7), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005be650), "c3");
		write_bytes(app_pid, NO_ASLR(0x005c0eac), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c0ed0), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c1250), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c1255), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1279), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c132c), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c1331), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1351), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c13ad), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c13b2), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1441), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1461), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c146e), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c1473), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1493), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c14a0), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c14a5), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c14dd), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1931), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c1955), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c1ca4), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c1ca9), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c373d), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x005c3761), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c3a44), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x005c3a49), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x00402c93), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x00402cb7), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x00402d5e), "9090909090");
		write_bytes(app_pid, NO_ASLR(0x00402d90), "31c0c3");
		write_bytes(app_pid, NO_ASLR(0x0054c0a3), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x008f56d0), "b801000000");
		printf_notification("Applied Debug Menu");
		// 120hz
		/*
		LEA RAX,[0x1262238]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x5610
		MOV EDI,dword ptr [0x01727230]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,0x1
		JNZ 0x00f1fcad
		LEA RAX,[0x1262238]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x1840
		MOV EDI,dword ptr [0x01727230]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		XOR EDI,EDI
		CALL 0x008e8720
		XOR EAX,EAX
		POP RBP
		RET
		*/
		// 120Hz
		// if (g_isPatch120Hz)
		{
			write_bytes(app_pid, NO_ASLR(0x00986279), "e9e2995900");
			write_bytes(app_pid, NO_ASLR(0x00f1fc60), "488d05d1253400");
			write_bytes(app_pid, NO_ASLR(0x00f1fc67), "488b00");
			write_bytes(app_pid, NO_ASLR(0x00f1fc6a), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x00f1fc70), "8b3dba758000");
			write_bytes(app_pid, NO_ASLR(0x00f1fc76), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x00f1fc7b), "31d2");
			write_bytes(app_pid, NO_ASLR(0x00f1fc7d), "31c9");
			write_bytes(app_pid, NO_ASLR(0x00f1fc7f), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x00f1fc82), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x00f1fc84), "83f801");
			write_bytes(app_pid, NO_ASLR(0x00f1fc87), "7524");
			write_bytes(app_pid, NO_ASLR(0x00f1fc89), "488d05a8253400");
			write_bytes(app_pid, NO_ASLR(0x00f1fc90), "488b00");
			write_bytes(app_pid, NO_ASLR(0x00f1fc93), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x00f1fc99), "8b3d91758000");
			write_bytes(app_pid, NO_ASLR(0x00f1fc9f), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x00f1fca4), "31d2");
			write_bytes(app_pid, NO_ASLR(0x00f1fca6), "31c9");
			write_bytes(app_pid, NO_ASLR(0x00f1fca8), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x00f1fcab), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x00f1fcad), "31ff");
			write_bytes(app_pid, NO_ASLR(0x00f1fcaf), "e86c8a9cff");
			write_bytes(app_pid, NO_ASLR(0x00f1fcb4), "31c0");
			write_bytes(app_pid, NO_ASLR(0x00f1fcb6), "5d");
			write_bytes(app_pid, NO_ASLR(0x00f1fcb7), "c3");
			// double speed videos fix
			write_bytes(app_pid, NO_ASLR(0x00930199), "e95ef95e00");
			write_bytes(app_pid, NO_ASLR(0x0093079a), "e988f35e00");
			write_bytes(app_pid, NO_ASLR(0x00f1fafc), "488d3d2d778000");
			write_bytes(app_pid, NO_ASLR(0x00f1fb03), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x00f1fb05), "488d0594e56300");
			write_bytes(app_pid, NO_ASLR(0x00f1fb0c), "83782c00");
			write_bytes(app_pid, NO_ASLR(0x00f1fb10), "be01000000");
			write_bytes(app_pid, NO_ASLR(0x00f1fb15), "e8860cffff");
			write_bytes(app_pid, NO_ASLR(0x00f1fb1a), "4c89e7");
			write_bytes(app_pid, NO_ASLR(0x00f1fb1d), "e84ef4a0ff");
			write_bytes(app_pid, NO_ASLR(0x00f1fb22), "e97706a1ff");
			write_bytes(app_pid, NO_ASLR(0x00f1fb27), "488d3d02778000");
			write_bytes(app_pid, NO_ASLR(0x00f1fb2e), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x00f1fb30), "488d0569e56300");
			write_bytes(app_pid, NO_ASLR(0x00f1fb37), "83782c00");
			write_bytes(app_pid, NO_ASLR(0x00f1fb3b), "31f6");
			write_bytes(app_pid, NO_ASLR(0x00f1fb3d), "e85e0cffff");
			write_bytes(app_pid, NO_ASLR(0x00f1fb42), "488b03");
			write_bytes(app_pid, NO_ASLR(0x00f1fb45), "483b45d0");
			write_bytes(app_pid, NO_ASLR(0x00f1fb49), "0f85e411a1ff");
			write_bytes(app_pid, NO_ASLR(0x00f1fb4f), "4c89e7");
			write_bytes(app_pid, NO_ASLR(0x00f1fb52), "4881c418010000");
			write_bytes(app_pid, NO_ASLR(0x00f1fb59), "5b");
			write_bytes(app_pid, NO_ASLR(0x00f1fb5a), "415c");
			write_bytes(app_pid, NO_ASLR(0x00f1fb5c), "415d");
			write_bytes(app_pid, NO_ASLR(0x00f1fb5e), "415e");
			write_bytes(app_pid, NO_ASLR(0x00f1fb60), "415f");
			write_bytes(app_pid, NO_ASLR(0x00f1fb62), "5d");
			write_bytes(app_pid, NO_ASLR(0x00f1fb63), "e928eba0ff");
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	case (0x100 << 3):
	{
		write_bytes(app_pid, NO_ASLR(0x009d86a2), "41c686ba39000000");
		printf_notification("Applied Debug Menu (Partial)");
		// 120hz
		/*
		LEA RAX,[0x17f5cc0]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x5610
		MOV EDI,dword ptr [0x01c0ef40]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,0x1
		JNZ 0x013491fd
		LEA RAX,[0x17f5cc0]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x1840
		MOV EDI,dword ptr [0x01c0ef40]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		XOR EDI,EDI
		CALL 0x0089bfe0
		XOR EAX,EAX
		POP RBP
		RET
		*/
		// 120Hz
		// if (g_isPatch120Hz)
		{
			write_bytes(app_pid, NO_ASLR(0x00931af9), "e9b276a100");
			write_bytes(app_pid, NO_ASLR(0x013491b0), "488d0509cb4a00");
			write_bytes(app_pid, NO_ASLR(0x013491b7), "488b00");
			write_bytes(app_pid, NO_ASLR(0x013491ba), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x013491c0), "8b3d7a5d8c00");
			write_bytes(app_pid, NO_ASLR(0x013491c6), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x013491cb), "31d2");
			write_bytes(app_pid, NO_ASLR(0x013491cd), "31c9");
			write_bytes(app_pid, NO_ASLR(0x013491cf), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x013491d2), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x013491d4), "83f801");
			write_bytes(app_pid, NO_ASLR(0x013491d7), "7524");
			write_bytes(app_pid, NO_ASLR(0x013491d9), "488d05e0ca4a00");
			write_bytes(app_pid, NO_ASLR(0x013491e0), "488b00");
			write_bytes(app_pid, NO_ASLR(0x013491e3), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x013491e9), "8b3d515d8c00");
			write_bytes(app_pid, NO_ASLR(0x013491ef), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x013491f4), "31d2");
			write_bytes(app_pid, NO_ASLR(0x013491f6), "31c9");
			write_bytes(app_pid, NO_ASLR(0x013491f8), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x013491fb), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x013491fd), "31ff");
			write_bytes(app_pid, NO_ASLR(0x013491ff), "e8dc2d55ff");
			write_bytes(app_pid, NO_ASLR(0x01349204), "31c0");
			write_bytes(app_pid, NO_ASLR(0x01349206), "5d");
			write_bytes(app_pid, NO_ASLR(0x01349207), "c3");
			// double speed videos fix
			write_bytes(app_pid, NO_ASLR(0x008e3a69), "e9fecfa500");
			write_bytes(app_pid, NO_ASLR(0x008e406a), "e928caa500");
			write_bytes(app_pid, NO_ASLR(0x01340a6c), "488d3dcde48c00");
			write_bytes(app_pid, NO_ASLR(0x01340a73), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x01340a75), "488d05f47f7000");
			write_bytes(app_pid, NO_ASLR(0x01340a7c), "83782c00");
			write_bytes(app_pid, NO_ASLR(0x01340a80), "be01000000");
			write_bytes(app_pid, NO_ASLR(0x01340a85), "e886ebfeff");
			write_bytes(app_pid, NO_ASLR(0x01340a8a), "4c89e7");
			write_bytes(app_pid, NO_ASLR(0x01340a8d), "e8ae1d5aff");
			write_bytes(app_pid, NO_ASLR(0x01340a92), "e9d72f5aff");
			write_bytes(app_pid, NO_ASLR(0x01340a97), "488d3da2e48c00");
			write_bytes(app_pid, NO_ASLR(0x01340a9e), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x01340aa0), "488d05c97f7000");
			write_bytes(app_pid, NO_ASLR(0x01340aa7), "83782c00");
			write_bytes(app_pid, NO_ASLR(0x01340aab), "31f6");
			write_bytes(app_pid, NO_ASLR(0x01340aad), "e85eebfeff");
			write_bytes(app_pid, NO_ASLR(0x01340ab2), "488b03");
			write_bytes(app_pid, NO_ASLR(0x01340ab5), "483b45d0");
			write_bytes(app_pid, NO_ASLR(0x01340ab9), "0f85343b5aff");
			write_bytes(app_pid, NO_ASLR(0x01340abf), "4c89e7");
			write_bytes(app_pid, NO_ASLR(0x01340ac2), "4881c438010000");
			write_bytes(app_pid, NO_ASLR(0x01340ac9), "5b");
			write_bytes(app_pid, NO_ASLR(0x01340aca), "415c");
			write_bytes(app_pid, NO_ASLR(0x01340acc), "415d");
			write_bytes(app_pid, NO_ASLR(0x01340ace), "415e");
			write_bytes(app_pid, NO_ASLR(0x01340ad0), "415f");
			write_bytes(app_pid, NO_ASLR(0x01340ad2), "5d");
			write_bytes(app_pid, NO_ASLR(0x01340ad3), "e988145aff");
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	case (0x102 << 1):
	{
		// Debug Menu
		write_bytes(app_pid, NO_ASLR(0x009cf259), "41c686ba39000000");
		printf_notification("Applied Debug Menu (Partial)");
		// 120Hz
		// if (g_isPatch120Hz)
		{
			/*
				LEA RAX,[0x00d4b888]
				MOV RAX,qword ptr [RAX]
				ADD RAX,0xca7f00d
				MOV EDI,dword ptr [0x015a21a8]
				MOV ESI,0xf
				XOR EDX,EDX
				XOR ECX,ECX
				XOR R8D,R8D
				CALL RAX
				CMP EAX,0x1
				JNZ exit
				LEA RAX,[0x00d4b888]
				MOV RAX,qword ptr [RAX]
				ADD RAX,0x5eedf00d
				MOV EDI,dword ptr [0x015a21a8]
				MOV ESI,0xf
				XOR EDX,EDX
				XOR ECX,ECX
				XOR R8D,R8D
				CALL RAX
				exit:
				MOV qword ptr [RBP + -0x8],0x0
				LEA RDI,[RBP + -0x8]
				CALL 0x00b69100
			*/
			write_bytes(app_pid, NO_ASLR(0x009145fb), "e974872f00");
			write_bytes(app_pid, NO_ASLR(0x00c0cd74), "488d050deb1300");
			write_bytes(app_pid, NO_ASLR(0x00c0cd7b), "488b00");
			write_bytes(app_pid, NO_ASLR(0x00c0cd7e), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x00c0cd84), "8b3d1e549900");
			write_bytes(app_pid, NO_ASLR(0x00c0cd8a), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x00c0cd8f), "31d2");
			write_bytes(app_pid, NO_ASLR(0x00c0cd91), "31c9");
			write_bytes(app_pid, NO_ASLR(0x00c0cd93), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x00c0cd96), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x00c0cd98), "83f801");
			write_bytes(app_pid, NO_ASLR(0x00c0cd9b), "7524");
			write_bytes(app_pid, NO_ASLR(0x00c0cd9d), "488d05e4ea1300");
			write_bytes(app_pid, NO_ASLR(0x00c0cda4), "488b00");
			write_bytes(app_pid, NO_ASLR(0x00c0cda7), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x00c0cdad), "8b3df5539900");
			write_bytes(app_pid, NO_ASLR(0x00c0cdb3), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x00c0cdb8), "31d2");
			write_bytes(app_pid, NO_ASLR(0x00c0cdba), "31c9");
			write_bytes(app_pid, NO_ASLR(0x00c0cdbc), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x00c0cdbf), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x00c0cdc1), "48c745f800000000");
			write_bytes(app_pid, NO_ASLR(0x00c0cdc9), "488d7df8");
			write_bytes(app_pid, NO_ASLR(0x00c0cdcd), "e82ec3f5ff");
			// double speed video fix
			write_bytes(app_pid, NO_ASLR(0x008a20a5), "e997ac3600");
			write_bytes(app_pid, NO_ASLR(0x008a3790), "e98d953600");
			write_bytes(app_pid, NO_ASLR(0x00c0cd22), "488d3d7f549900");
			write_bytes(app_pid, NO_ASLR(0x00c0cd29), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x00c0cd2b), "be01000000");
			write_bytes(app_pid, NO_ASLR(0x00c0cd30), "e8bbd0f5ff");
			write_bytes(app_pid, NO_ASLR(0x00c0cd35), "4d8db5c0280200");
			write_bytes(app_pid, NO_ASLR(0x00c0cd3c), "e9566ac9ff");
			write_bytes(app_pid, NO_ASLR(0x00c0cd41), "488d3d60549900");
			write_bytes(app_pid, NO_ASLR(0x00c0cd48), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x00c0cd4a), "31f6");
			write_bytes(app_pid, NO_ASLR(0x00c0cd4c), "e89fd0f5ff");
			write_bytes(app_pid, NO_ASLR(0x00c0cd51), "4c89ef");
			write_bytes(app_pid, NO_ASLR(0x00c0cd54), "e8f745c9ff");
			write_bytes(app_pid, NO_ASLR(0x00c0cd59), "e94f53c9ff");
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	case (0x102 << 2):
	{
		// Debug Menu
		write_bytes(app_pid, NO_ASLR(0x010e22fe), "41c686ba39000000");
		printf_notification("Applied Debug Menu (Partial)");
		// 120Hz
		// if (g_isPatch120Hz)
		{
			/*
				LEA RAX,[0x0144bae0]
				MOV RAX,qword ptr [RAX]
				ADD RAX,0xca7f00d
				MOV EDI,dword ptr [0x01a73428]
				MOV ESI,0xf
				XOR EDX,EDX
				XOR ECX,ECX
				XOR R8D,R8D
				CALL RAX
				CMP EAX,0x1
				JNZ exit
				LEA RAX,[0x0144bae0]
				MOV RAX,qword ptr [RAX]
				ADD RAX,0x5eedf00d
				MOV EDI,dword ptr [0x01a73428]
				MOV ESI,0xf
				XOR EDX,EDX
				XOR ECX,ECX
				XOR R8D,R8D
				CALL RAX
				exit:
				MOV qword ptr [RBP + -0x8],0x0
				LEA RDI,[RBP + -0x8]
				CALL 0x011e6a80
			*/
			write_bytes(app_pid, NO_ASLR(0x0101404b), "e9aafa2900");
			write_bytes(app_pid, NO_ASLR(0x012b3afa), "488d05df7f1900");
			write_bytes(app_pid, NO_ASLR(0x012b3b01), "488b00");
			write_bytes(app_pid, NO_ASLR(0x012b3b04), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x012b3b0a), "8b3d18f97b00");
			write_bytes(app_pid, NO_ASLR(0x012b3b10), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x012b3b15), "31d2");
			write_bytes(app_pid, NO_ASLR(0x012b3b17), "31c9");
			write_bytes(app_pid, NO_ASLR(0x012b3b19), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x012b3b1c), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x012b3b1e), "83f801");
			write_bytes(app_pid, NO_ASLR(0x012b3b21), "7524");
			write_bytes(app_pid, NO_ASLR(0x012b3b23), "488d05b67f1900");
			write_bytes(app_pid, NO_ASLR(0x012b3b2a), "488b00");
			write_bytes(app_pid, NO_ASLR(0x012b3b2d), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x012b3b33), "8b3deff87b00");
			write_bytes(app_pid, NO_ASLR(0x012b3b39), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x012b3b3e), "31d2");
			write_bytes(app_pid, NO_ASLR(0x012b3b40), "31c9");
			write_bytes(app_pid, NO_ASLR(0x012b3b42), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x012b3b45), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x012b3b47), "48c745f800000000");
			write_bytes(app_pid, NO_ASLR(0x012b3b4f), "488d7df8");
			write_bytes(app_pid, NO_ASLR(0x012b3b53), "e8282ff3ff");
			/*
				start:
				LEA RDI, [0x01a73428]
				MOV EDI,dword ptr [RDI]
				MOV ESI, 1
				CALL 0x011e7250
				LEA R14,[R12 + 0x228c0]
				JMP 0x00f870de
				exit:
				LEA RDI, [0x01a73428]
				MOV EDI,dword ptr [RDI]
				XOR ESI, ESI
				CALL 0x011e7250
				MOV RDI,R12
				CALL 0x00f84cf0
				JMP 0x00f85a20
			*/
			// double speed videos fix
			write_bytes(app_pid, NO_ASLR(0x00f85a18), "e9abe03200");
			write_bytes(app_pid, NO_ASLR(0x00f870d6), "e9cdc93200");
			write_bytes(app_pid, NO_ASLR(0x012b3aa8), "488d3d79f97b00");
			write_bytes(app_pid, NO_ASLR(0x012b3aaf), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x012b3ab1), "be01000000");
			write_bytes(app_pid, NO_ASLR(0x012b3ab6), "e89537f3ff");
			write_bytes(app_pid, NO_ASLR(0x012b3abb), "4d8db424c0280200");
			write_bytes(app_pid, NO_ASLR(0x012b3ac3), "e91636cdff");
			write_bytes(app_pid, NO_ASLR(0x012b3ac8), "488d3d59f97b00");
			write_bytes(app_pid, NO_ASLR(0x012b3acf), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x012b3ad1), "31f6");
			write_bytes(app_pid, NO_ASLR(0x012b3ad3), "e87837f3ff");
			write_bytes(app_pid, NO_ASLR(0x012b3ad8), "4c89e7");
			write_bytes(app_pid, NO_ASLR(0x012b3adb), "e81012cdff");
			write_bytes(app_pid, NO_ASLR(0x012b3ae0), "e93b1fcdff");
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	case (0x102 << 3):
	{
		// Debug Menu
		write_bytes(app_pid, NO_ASLR(0x01337ac9), "41c686ba39000000");
		printf_notification("Applied Debug Menu (Partial)");
		// 120hz
		// if (g_isPatch120Hz)
		{
			/*
				LEA RAX,[0x01ae3d78]
				MOV RAX,qword ptr [RAX]
				ADD RAX,0xca7f00d
				MOV EDI,dword ptr [0x020f5338]
				MOV ESI,0xf
				XOR EDX,EDX
				XOR ECX,ECX
				XOR R8D,R8D
				CALL RAX
				CMP EAX,0x1
				JNZ exit
				LEA RAX,[0x01ae3d78]
				MOV RAX,qword ptr [RAX]
				ADD RAX,0x5eedf00d
				MOV EDI,dword ptr [0x020f5338]
				MOV ESI,0xf
				XOR EDX,EDX
				XOR ECX,ECX
				XOR R8D,R8D
				CALL RAX
				exit:
				MOV qword ptr [RBP + -0x8],0x0
				LEA RDI,[RBP + -0x8]
				CALL 0x0179cc10
			*/
			write_bytes(app_pid, NO_ASLR(0x012766db), "e91fe26000");
			write_bytes(app_pid, NO_ASLR(0x018848ff), "488d0572f42500");
			write_bytes(app_pid, NO_ASLR(0x01884906), "488b00");
			write_bytes(app_pid, NO_ASLR(0x01884909), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x0188490f), "8b3d230a8700");
			write_bytes(app_pid, NO_ASLR(0x01884915), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x0188491a), "31d2");
			write_bytes(app_pid, NO_ASLR(0x0188491c), "31c9");
			write_bytes(app_pid, NO_ASLR(0x0188491e), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x01884921), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x01884923), "83f801");
			write_bytes(app_pid, NO_ASLR(0x01884926), "7524");
			write_bytes(app_pid, NO_ASLR(0x01884928), "488d0549f42500");
			write_bytes(app_pid, NO_ASLR(0x0188492f), "488b00");
			write_bytes(app_pid, NO_ASLR(0x01884932), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x01884938), "8b3dfa098700");
			write_bytes(app_pid, NO_ASLR(0x0188493e), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x01884943), "31d2");
			write_bytes(app_pid, NO_ASLR(0x01884945), "31c9");
			write_bytes(app_pid, NO_ASLR(0x01884947), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x0188494a), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x0188494c), "48c745f800000000");
			write_bytes(app_pid, NO_ASLR(0x01884954), "488d7df8");
			write_bytes(app_pid, NO_ASLR(0x01884958), "e8b382f1ff");
			/*
				start:
				LEA RDI, [0x020f5338]
				MOV EDI,dword ptr [RDI]
				MOV ESI, 1
				CALL 0x0179d2b0
				LEA R14,[R12 + 0x228c0]
				JMP 0x011f26a0
				exit:
				LEA RDI, [0x020f5338]
				MOV EDI,dword ptr [RDI]
				XOR ESI, ESI
				CALL 0x0179d2b0
				MOV RDI,R12
				CALL 0x011f0240
				JMP 0x011f0fe0
			*/
			// double speed videos fix
			write_bytes(app_pid, NO_ASLR(0x011f0fd8), "e9f0386900");
			write_bytes(app_pid, NO_ASLR(0x011f2698), "e910226900");
			write_bytes(app_pid, NO_ASLR(0x018848ad), "488d3d840a8700");
			write_bytes(app_pid, NO_ASLR(0x018848b4), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x018848b6), "be01000000");
			write_bytes(app_pid, NO_ASLR(0x018848bb), "e8f089f1ff");
			write_bytes(app_pid, NO_ASLR(0x018848c0), "4d8db424c0280200");
			write_bytes(app_pid, NO_ASLR(0x018848c8), "e9d3dd96ff");
			write_bytes(app_pid, NO_ASLR(0x018848cd), "488d3d640a8700");
			write_bytes(app_pid, NO_ASLR(0x018848d4), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x018848d6), "31f6");
			write_bytes(app_pid, NO_ASLR(0x018848d8), "e8d389f1ff");
			write_bytes(app_pid, NO_ASLR(0x018848dd), "4c89e7");
			write_bytes(app_pid, NO_ASLR(0x018848e0), "e85bb996ff");
			write_bytes(app_pid, NO_ASLR(0x018848e5), "e9f6c696ff");
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	}
}

void DoPatch_TheOrder1886_102(pid_t app_pid, uint64_t text_base)
{
	// 60 FPS
	if (parseXML(TO1886_60FPS))
	{
		// TODO: Fix QTEs
		write_bytes(app_pid, NO_ASLR(0x008507c4), "01");
		printf_notification("Applied 60 FPS");
	}
	if (parseXML(TO1886_16_9))
	{
		// 900p + 16:9
		write_bytes(app_pid, NO_ASLR(0x0084cef5), "75");
		write_bytes32(app_pid, NO_ASLR(0x0084cefd), 1600);
		write_bytes32(app_pid, NO_ASLR(0x0084cf07), 900);
		// 16:9 AR
		write_bytes32(app_pid, NO_ASLR(0x0084ce8c), 0x3fe38e39);
		printf_notification("Applied 16:9");
	}
}

void DoPatch_Big4R_100(pid_t app_pid, uint64_t text_base, uint32_t idx)
{
	switch (idx)
	{
	case 1: // `eboot.bin`
	{
		// 75 2a 84 d2 74 26
		// write_bytes(app_pid, BASE_ASLR_OFFSET(0x01000000, 0x015b7650), "cc");
		if (parseXML(Big4R_MainDebugMenu))
		{
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x01000000, 0x01578838), "eb2a");
			printf_notification("Applied Debug Menu");
		}
		break;
	}
	case 2: // `tllr-boot.bin`
	{
		// 75 2a 84 d2 74 26
		if (parseXML(Big4R_TLLDebugMenu))
		{
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x01000000, 0x015783e8), "eb2a");
			printf_notification("Applied Debug Menu");
		}
		break;
	}
	}
}

void DoPatch_DemonSouls(pid_t app_pid, uint64_t text_base, uint32_t idx)
{
	switch (idx)
	{
	case 0x100:
	{
		/*
		LEA RAX,[0x3890fe10]
		MOV RAX,qword ptr [RAX]
		ADD RAX,-0x1
		MOV EDI,dword ptr [RBX + 0x98]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,0x1
		JNZ 0x3776ad97
		LEA RAX,[0x3890fe10]
		MOV RAX,qword ptr [RAX]
		SUB RAX,-0x1
		MOV EDI,dword ptr [RBX + 0x98]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		JMP 0x36719091
		*/
		// 120Hz
		if (g_isPatch120Hz)
		{
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3671908c), "e9b91c0501");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad4a), "488d05a7501a01");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad51), "488b00");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad54), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad5a), "8bbb98000000");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad60), "be0f000000");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad65), "31d2");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad67), "31c9");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad69), "4531c0");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad6c), "ffd0");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad6e), "83f801");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad71), "7524");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad73), "488d057e501a01");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad7a), "488b00");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad7d), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad83), "8bbb98000000");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad89), "be0f000000");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad8e), "31d2");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad90), "31c9");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad92), "4531c0");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad95), "ffd0");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x3776ad97), "e9f5e2fafe");
			printf_notification("Applied 120Hz Patch");
		}
		if (parseXML(DemonSouls_UnlockFPS))
		{
			// use vsync
			write_bytes(app_pid, BASE_ASLR(0x023eddc0), "31f6");
			write_bytes(app_pid, BASE_ASLR(0x023eddc2), "31d2");
			write_bytes(app_pid, BASE_ASLR(0x023eddc4), "ff254e609d00");
			// no fliprate
			write_bytes(app_pid, BASE_ASLR(0x023eddb0), "31f6");
			write_bytes(app_pid, BASE_ASLR(0x023eddb2), "ff2558609d00");
			printf_notification("Applied 60 FPS");
		}
		if (parseXML(DemonSouls_DebugMenu))
		{
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x35b4c0bf), "cc");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x35b4c0c0), "488b03");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x35b4c0c3), "483b4308");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x35b4c0c7), "c60532b7530302");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x35b4c0ce), "eb13");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x35b4c0e3), "e9cd4bae00");
			// Preserve `bpe::CommandLine::DispatchCmds`
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x35b4c000, 0x36630cae), "e90db451ff");
			printf_notification("Applied Debug Menu");
		}
		break;
	}
	case 0x104:
	{
		if (parseXML(DemonSouls104_DebugMenu))
		{
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x584a8000, 0x58cfbb49), "e937010000");
			// Preserve `bpe::CommandLine::DispatchCmds`
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x584a8000, 0x58cfbc85), "e8b676ffff");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x584a8000, 0x58cfbc8a), "e9e4000000");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x584a8000, 0x58cfbd73), "c60526058b0202");
			write_bytes(app_pid, BASE_ASLR_OFFSET(0x584a8000, 0x58cfbd7a), "e9cffdffff");
			printf_notification("Applied Debug Menu");
		}
		break;
	}
	}
}

void DoPatch_DriveClub_128(pid_t app_pid, uint64_t text_base)
{
	// 60 FPS
	if (parseXML(Driveclub_60FPS))
	{
		write_bytes(app_pid, NO_ASLR(0x01184800), "b801000000");
		write_bytes(app_pid, NO_ASLR(0x01184725), "48b001");
		write_bytes(app_pid, NO_ASLR(0x00a6306e), "9090");
		printf_notification("Applied 60 FPS");
	}
	// Unlock All Cars + Bikes
	if (parseXML(Driveclub_DLC_Unlock))
	{
		write_bytes(app_pid, NO_ASLR(0x01187caf), "9090");
		write_bytes(app_pid, NO_ASLR(0x01187ce7), "b801000000");
		printf_notification("Applied Unlock All Cars + Bikes");
	}
}

void DoPatch_TheLastGuardian_103(pid_t app_pid, uint64_t text_base)
{
	// 60 FPS
	if (parseXML(TLG_60FPS))
	{
		write_bytes(app_pid, NO_ASLR(0x0105151a), "be00000000");
		/*
		_start:
			MOV RDI,qword ptr [RBP + -0x40]
			CALL set_fliprate
			MOV RSI,qword ptr [RBP + -0x38]
			MOV RDI,qword ptr [RBP + -0x40]
			BEXTR RDX,RSI,R15
			CMP R13,RSI
			SBB ECX,ECX
			AND ECX,0x1
			JMP 0x0060bebd
		fliprate_thunk:
			jmp 0x018bdd78
		set_fliprate:
			push    rbp
			mov     rbp, rsp
			sub     rsp, 0x10
			mov     qword ptr [rbp - 0x8], rdi
			mov     rax, qword ptr [rbp - 0x8]
			mov   eax, dword ptr [rax]
			mov     dword ptr [rbp - 0xc], eax
			cmp     dword ptr [rbp - 0xc], 0x2F303173
			jne     LBB1_4
			mov     edi, dword ptr [0x05bd6b24]
			xor esi, esi
			call    fliprate_thunk
			mov     rax, qword ptr [rbp - 0x8]
			mov   eax, dword ptr [rax + 0x11]
			mov     dword ptr [rbp - 0xc], eax
			cmp     dword ptr [rbp - 0xc], 0x30373030
			jne     LBB1_3
			mov     edi, dword ptr [0x05bd6b24]
			mov     esi, 1
			call    fliprate_thunk
			mov dword ptr [0x01b04014], 0xca7f00d
			jmp     LBB1_4
		LBB1_3:
			mov dword ptr [0x01b04014], 0x5eaf00d
			jmp     LBB1_4
		LBB1_4:
			add     rsp, 0x10
			pop     rbp
			ret
		save_checkpoint_hook:
			PUSH RBP
			MOV RBP,RSP
			cmp dword ptr [0x01b04014], 0xca7f00d
			jnz orig
			mov     edi, dword ptr [0x05bd6b24]
			xor esi, esi
			call    fliprate_thunk
			lea rax, [0x01b04014]
			mov dword ptr [rax], 0x5eaf00d
		orig:
			LEA RAX, [0x05c32568]
			MOV RAX,qword ptr [RAX]
			MOV RDI,qword ptr [RAX + 0x108]
			CALL 0x00ecb540
			XOR EAX,EAX
			POP RBP
			RET
		*/
		write_bytes(app_pid, NO_ASLR(0x0060b150), "e98b982c01");
		write_bytes(app_pid, NO_ASLR(0x0060bea8), "e99f8a2c01");
		write_bytes(app_pid, NO_ASLR(0x018d494c), "488b7dc0");
		write_bytes(app_pid, NO_ASLR(0x018d4950), "e81f000000");
		write_bytes(app_pid, NO_ASLR(0x018d4955), "488b75c8");
		write_bytes(app_pid, NO_ASLR(0x018d4959), "488b7dc0");
		write_bytes(app_pid, NO_ASLR(0x018d495d), "c4e280f7d6");
		write_bytes(app_pid, NO_ASLR(0x018d4962), "4939f5");
		write_bytes(app_pid, NO_ASLR(0x018d4965), "19c9");
		write_bytes(app_pid, NO_ASLR(0x018d4967), "83e101");
		write_bytes(app_pid, NO_ASLR(0x018d496a), "e94e75d3fe");
		write_bytes(app_pid, NO_ASLR(0x018d496f), "e90494feff");
		write_bytes(app_pid, NO_ASLR(0x018d4974), "55");
		write_bytes(app_pid, NO_ASLR(0x018d4975), "4889e5");
		write_bytes(app_pid, NO_ASLR(0x018d4978), "4883ec10");
		write_bytes(app_pid, NO_ASLR(0x018d497c), "48897df8");
		write_bytes(app_pid, NO_ASLR(0x018d4980), "488b45f8");
		write_bytes(app_pid, NO_ASLR(0x018d4984), "8b00");
		write_bytes(app_pid, NO_ASLR(0x018d4986), "8945f4");
		write_bytes(app_pid, NO_ASLR(0x018d4989), "817df47331302f");
		write_bytes(app_pid, NO_ASLR(0x018d4990), "7548");
		write_bytes(app_pid, NO_ASLR(0x018d4992), "8b3d8c213004");
		write_bytes(app_pid, NO_ASLR(0x018d4998), "31f6");
		write_bytes(app_pid, NO_ASLR(0x018d499a), "e8d0ffffff");
		write_bytes(app_pid, NO_ASLR(0x018d499f), "488b45f8");
		write_bytes(app_pid, NO_ASLR(0x018d49a3), "8b4011");
		write_bytes(app_pid, NO_ASLR(0x018d49a6), "8945f4");
		write_bytes(app_pid, NO_ASLR(0x018d49a9), "817df430303730");
		write_bytes(app_pid, NO_ASLR(0x018d49b0), "751c");
		write_bytes(app_pid, NO_ASLR(0x018d49b2), "8b3d6c213004");
		write_bytes(app_pid, NO_ASLR(0x018d49b8), "be01000000");
		write_bytes(app_pid, NO_ASLR(0x018d49bd), "e8adffffff");
		write_bytes(app_pid, NO_ASLR(0x018d49c2), "c70548f622000df0a70c");
		write_bytes(app_pid, NO_ASLR(0x018d49cc), "eb0c");
		write_bytes(app_pid, NO_ASLR(0x018d49ce), "c7053cf622000df0ea05");
		write_bytes(app_pid, NO_ASLR(0x018d49d8), "eb00");
		write_bytes(app_pid, NO_ASLR(0x018d49da), "4883c410");
		write_bytes(app_pid, NO_ASLR(0x018d49de), "5d");
		write_bytes(app_pid, NO_ASLR(0x018d49df), "c3");
		write_bytes(app_pid, NO_ASLR(0x018d49e0), "55");
		write_bytes(app_pid, NO_ASLR(0x018d49e1), "4889e5");
		write_bytes(app_pid, NO_ASLR(0x018d49e4), "813d26f622000df0a70c");
		write_bytes(app_pid, NO_ASLR(0x018d49ee), "751a");
		write_bytes(app_pid, NO_ASLR(0x018d49f0), "8b3d2e213004");
		write_bytes(app_pid, NO_ASLR(0x018d49f6), "31f6");
		write_bytes(app_pid, NO_ASLR(0x018d49f8), "e872ffffff");
		write_bytes(app_pid, NO_ASLR(0x018d49fd), "488d0510f62200");
		write_bytes(app_pid, NO_ASLR(0x018d4a04), "c7000df0ea05");
		write_bytes(app_pid, NO_ASLR(0x018d4a0a), "488d0557db3504");
		write_bytes(app_pid, NO_ASLR(0x018d4a11), "488b00");
		write_bytes(app_pid, NO_ASLR(0x018d4a14), "488bb808010000");
		write_bytes(app_pid, NO_ASLR(0x018d4a1b), "e8206b5fff");
		write_bytes(app_pid, NO_ASLR(0x018d4a20), "31c0");
		write_bytes(app_pid, NO_ASLR(0x018d4a22), "5d");
		write_bytes(app_pid, NO_ASLR(0x018d4a23), "c3");
		printf_notification("Applied 60 FPS + Freeze Fix");
	}
	// Native 3840x2160
	// write_bytes64(app_pid, NO_ASLR(0x01aec3e8), 0x08700f0008700f00);
	// write_bytes64(app_pid, NO_ASLR(0x01aec3f0), 0x0000000000000000);
	if (g_isPatch120Hz)
	{
		// force 1080p on 4k display
		write_bytes64(app_pid, NO_ASLR(0x01aec3e8), 0x0438078004380780);
		write_bytes64(app_pid, NO_ASLR(0x01aec3f0), 0x0000000000000000);
		// 120hz
		write_bytes(app_pid, NO_ASLR(0x01051968), "e97db89500");
		write_bytes(app_pid, NO_ASLR(0x019ad1ea), "488d05ff371500");
		write_bytes(app_pid, NO_ASLR(0x019ad1f1), "488b00");
		write_bytes(app_pid, NO_ASLR(0x019ad1f4), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x019ad1fa), "488d3d23992204");
		write_bytes(app_pid, NO_ASLR(0x019ad201), "8b3f");
		write_bytes(app_pid, NO_ASLR(0x019ad203), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x019ad208), "31d2");
		write_bytes(app_pid, NO_ASLR(0x019ad20a), "31c9");
		write_bytes(app_pid, NO_ASLR(0x019ad20c), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x019ad20f), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x019ad211), "83f801");
		write_bytes(app_pid, NO_ASLR(0x019ad214), "7527");
		write_bytes(app_pid, NO_ASLR(0x019ad216), "488d05d3371500");
		write_bytes(app_pid, NO_ASLR(0x019ad21d), "488b00");
		write_bytes(app_pid, NO_ASLR(0x019ad220), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x019ad226), "488d3df7982204");
		write_bytes(app_pid, NO_ASLR(0x019ad22d), "8b3f");
		write_bytes(app_pid, NO_ASLR(0x019ad22f), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x019ad234), "31d2");
		write_bytes(app_pid, NO_ASLR(0x019ad236), "31c9");
		write_bytes(app_pid, NO_ASLR(0x019ad238), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x019ad23b), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x019ad23d), "4889d8");
		write_bytes(app_pid, NO_ASLR(0x019ad240), "bff0300100");
		write_bytes(app_pid, NO_ASLR(0x019ad245), "e923476aff");
		printf_notification("Applied 120Hz Patch");
	}
}

void DoPatch_CTR_121(pid_t app_pid, uint64_t text_base, uint32_t idx)
{
	switch (idx)
	{
	case 1:
	{
		write_bytes(app_pid, NO_ASLR(0x017a9f2b), "be00000000");
		write_bytes(app_pid, NO_ASLR(0x017a9f30), "9090909090909090909090909090");
		printf_notification("Applied 60 FPS");
		break;
	}
	case 2:
	{
		write_bytes(app_pid, NO_ASLR(0x017a9f1b), "be00000000");
		write_bytes(app_pid, NO_ASLR(0x017a9f20), "9090909090909090909090909090");
		printf_notification("Applied 60 FPS");
		break;
	}
	}
}

void DoPatch_t1ps4_111(pid_t app_pid, uint64_t text_base)
{
	// Debug Menu
	write_bytes(app_pid, NO_ASLR(0x004021a4), "75");
	// Rendering Menu
	write_bytes(app_pid, NO_ASLR(0x0046df6c), "e84f6b8400");
	write_bytes(app_pid, NO_ASLR(0x00eeea79), "bfb0000000");
	write_bytes(app_pid, NO_ASLR(0x00eeea7e), "eb36");
	write_bytes(app_pid, NO_ASLR(0x00eeecd2), "bfb0000000");
	write_bytes(app_pid, NO_ASLR(0x00eeecd7), "e834231900");
	write_bytes(app_pid, NO_ASLR(0x00eeecdc), "4989c7");
	write_bytes(app_pid, NO_ASLR(0x00eeecdf), "488d35d42c6800");
	write_bytes(app_pid, NO_ASLR(0x00eeece6), "31d2");
	write_bytes(app_pid, NO_ASLR(0x00eeece8), "31c9");
	write_bytes(app_pid, NO_ASLR(0x00eeecea), "4c89ff");
	write_bytes(app_pid, NO_ASLR(0x00eeeced), "e80eecf3ff");
	write_bytes(app_pid, NO_ASLR(0x00eeecf2), "bfa0000000");
	write_bytes(app_pid, NO_ASLR(0x00eeecf7), "e814231900");
	write_bytes(app_pid, NO_ASLR(0x00eeecfc), "4989c4");
	write_bytes(app_pid, NO_ASLR(0x00eeecff), "488d35c52c6800");
	write_bytes(app_pid, NO_ASLR(0x00eeed06), "31c9");
	write_bytes(app_pid, NO_ASLR(0x00eeed08), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x00eeed0b), "4c89fa");
	write_bytes(app_pid, NO_ASLR(0x00eeed0e), "4c89e7");
	write_bytes(app_pid, NO_ASLR(0x00eeed11), "e8aae0f3ff");
	write_bytes(app_pid, NO_ASLR(0x00eeed16), "4c89f3");
	write_bytes(app_pid, NO_ASLR(0x00eeed19), "4c89e6");
	write_bytes(app_pid, NO_ASLR(0x00eeed1c), "4889df");
	write_bytes(app_pid, NO_ASLR(0x00eeed1f), "e89ceff3ff");
	write_bytes(app_pid, NO_ASLR(0x00eeed24), "eb27");
	write_bytes(app_pid, NO_ASLR(0x00eee530), "c3");
	// 60 FPS
	write_bytes(app_pid, NO_ASLR(0x01487140), "31f6");
	write_bytes(app_pid, NO_ASLR(0x01487142), "ff2580cb4800");
	// Skip Startup Logos
	write_bytes(app_pid, NO_ASLR(0x0040214a), "c687e534000001");
	// Give Player Weapon Fix
	write_bytes(app_pid, NO_ASLR(0x00475db0), "be01000000");
	write_bytes(app_pid, NO_ASLR(0x00475db5), "48893d597d8501");
	write_bytes(app_pid, NO_ASLR(0x00475dbc), "c3");
	write_bytes(app_pid, NO_ASLR(0x00475dbd), "488b07");
	write_bytes(app_pid, NO_ASLR(0x00475dc0), "488905597d8501");
	write_bytes(app_pid, NO_ASLR(0x00475dc7), "4831c0");
	write_bytes(app_pid, NO_ASLR(0x00475dca), "be01000000");
	write_bytes(app_pid, NO_ASLR(0x00475dcf), "c3");
	write_bytes(app_pid, NO_ASLR(0x00475dd0), "488305917d850120");
	write_bytes(app_pid, NO_ASLR(0x00475dd8), "488d3571d73d01");
	write_bytes(app_pid, NO_ASLR(0x00475ddf), "480335837d8501");
	write_bytes(app_pid, NO_ASLR(0x00475de6), "c3");
	write_bytes(app_pid, NO_ASLR(0x00919311), "488b35fd473b01");
	write_bytes(app_pid, NO_ASLR(0x00919392), "488b3d7c473b01");
	write_bytes(app_pid, NO_ASLR(0x00919189), "48e841ccb5ff");
	write_bytes(app_pid, NO_ASLR(0x0091918f), "90");
	write_bytes(app_pid, NO_ASLR(0x00919399), "e81fcab5ff");
	write_bytes(app_pid, NO_ASLR(0x009192c9), "e8e2cab5ff");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853570), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x01853590), "pistol-uber-t1                 ");
	write_string(app_pid, NO_ASLR(0x018535b0), "baseball-bat                   ");
	write_string(app_pid, NO_ASLR(0x018535d0), "hatchet                        ");
	write_string(app_pid, NO_ASLR(0x018535f0), "NO_ART_GROUP                   ");
	write_string(app_pid, NO_ASLR(0x01853610), "nail-bomb                      ");
	write_string(app_pid, NO_ASLR(0x01853630), "two-by-four                    ");
	write_string(app_pid, NO_ASLR(0x01853650), "nail-bomb                      ");
	write_string(app_pid, NO_ASLR(0x01853670), "turret-sniper                  ");
	write_string(app_pid, NO_ASLR(0x01853690), "rifle-lever-action             ");
	write_string(app_pid, NO_ASLR(0x018536b0), "pistol-uber-t1                 ");
	write_string(app_pid, NO_ASLR(0x018536d0), "shiv                           ");
	write_string(app_pid, NO_ASLR(0x018536f0), "radio                          ");
	write_string(app_pid, NO_ASLR(0x01853710), "molotov                        ");
	write_string(app_pid, NO_ASLR(0x01853730), "bandage-apply                  ");
	write_string(app_pid, NO_ASLR(0x01853750), "mp-parcel                      ");
	write_string(app_pid, NO_ASLR(0x01853770), "gren-launcher-improvised       ");
	write_string(app_pid, NO_ASLR(0x01853790), "shotgun-dbl-barrel             ");
	write_string(app_pid, NO_ASLR(0x018537b0), "machete-blade                  ");
	write_string(app_pid, NO_ASLR(0x018537d0), "bow-dark                       ");
	write_string(app_pid, NO_ASLR(0x018537f0), "rifle-variable                 ");
	write_string(app_pid, NO_ASLR(0x01853810), "mini-14-rifle                  ");
	write_string(app_pid, NO_ASLR(0x01853830), "two-by-four                    ");
	write_string(app_pid, NO_ASLR(0x01853850), "pistol-shotgun                 ");
	write_string(app_pid, NO_ASLR(0x01853870), "rifle-improvised-fullauto      ");
	write_string(app_pid, NO_ASLR(0x01853890), "super-soaker                   ");
	write_string(app_pid, NO_ASLR(0x018538b0), "NO_ART_GROUP                   ");
	write_string(app_pid, NO_ASLR(0x018538d0), "beer-bottle                    ");
	write_string(app_pid, NO_ASLR(0x018538f0), "crossbow-dlc                   ");
	write_string(app_pid, NO_ASLR(0x01853910), "lead-pipe                      ");
	write_string(app_pid, NO_ASLR(0x01853930), "lead-pipe                      ");
	write_string(app_pid, NO_ASLR(0x01853950), "flamethrower                   ");
	write_string(app_pid, NO_ASLR(0x01853970), "desert-eagle-t1                ");
	write_string(app_pid, NO_ASLR(0x01853990), "flashlight                     ");
	write_string(app_pid, NO_ASLR(0x018539b0), "rifle-sniper                   ");
	write_string(app_pid, NO_ASLR(0x018539d0), "m4-rifle-t1                    ");
	write_string(app_pid, NO_ASLR(0x018539f0), "smoke-bomb                     ");
	write_string(app_pid, NO_ASLR(0x01853a10), "throw-brick                    ");
	write_string(app_pid, NO_ASLR(0x01853a30), "shotgun-pump-stock             ");
	write_string(app_pid, NO_ASLR(0x01853a50), "silenced-galil-rifle           ");
	write_string(app_pid, NO_ASLR(0x01853a70), "NO_ART_GROUP                   ");
	write_string(app_pid, NO_ASLR(0x01853a90), "bloater-pustule                ");
	write_string(app_pid, NO_ASLR(0x01853ab0), "taurus-66b4-revolver           ");
	write_string(app_pid, NO_ASLR(0x01853ad0), "nail-bomb                      ");
	write_string(app_pid, NO_ASLR(0x01853af0), "rifle-improvised-assault       ");
	write_string(app_pid, NO_ASLR(0x01853b10), "throwing-knife                 ");
	write_string(app_pid, NO_ASLR(0x01853b30), "rifle-bolt                     ");
	write_string(app_pid, NO_ASLR(0x01853b50), "beretta-jaguar-pistol          ");
	write_string(app_pid, NO_ASLR(0x01853b70), "bow                            ");
	write_string(app_pid, NO_ASLR(0x01853b90), "molotov                        ");
	write_string(app_pid, NO_ASLR(0x01853bb0), "bill-knife                     ");
	write_string(app_pid, NO_ASLR(0x01853bd0), "baseball-bat                   ");
	write_string(app_pid, NO_ASLR(0x01853bf0), "mini-14-rifle                  ");
	write_string(app_pid, NO_ASLR(0x01853c10), "pistol-glock-burst             ");
	write_string(app_pid, NO_ASLR(0x01853c30), "pistol-9mm                     ");
	write_string(app_pid, NO_ASLR(0x01853c50), "shotgun-tactical               ");
	write_string(app_pid, NO_ASLR(0x01853c70), "mal-gas-can                    ");
	write_string(app_pid, NO_ASLR(0x01853c90), "machete-blade                  ");
	write_string(app_pid, NO_ASLR(0x01853cb0), "none                           ");
	write_string(app_pid, NO_ASLR(0x01853cd0), "rifle-improvised-assault       ");
	write_string(app_pid, NO_ASLR(0x01853cf0), "binoculars                     ");
	write_string(app_pid, NO_ASLR(0x01853d10), "switchblade                    ");
	write_string(app_pid, NO_ASLR(0x01853d30), "rifle-improvised-fullauto      ");
	// Draw Color Boarder
	/*
	Source:
	_start:
	# Start addr 0x00c94d40
	PUSH RBP
	MOV RBP,RSP
	PUSH R15
	PUSH R14
	PUSH R13
	PUSH R12
	PUSH RBX
	SUB RSP,0x78
	LEA RDI, [0x01c1a770]
	CMP byte ptr [RDI+0x7c0], 0
	JZ exit_func
	LEA RDI,[RBP + -0x88]
	MOV ESI,0x1
	XOR EDX,EDX
	CALL 0x00ec4b90 # Get window context
	vxorps XMM0,XMM0,XMM0
	vxorps XMM1,XMM1,XMM1
	vxorps XMM2,XMM2,XMM2
	vxorps XMM3,XMM3,XMM3
	# x1
	MOV R14D, 0x0
	VCVTSI2SS XMM0,XMM0,R14D
	# y1
	MOV R14D, 0x0
	VCVTSI2SS XMM1,XMM1,R14D
	# x2
	MOV R14D, 0x20
	VCVTSI2SS XMM2,XMM2,R14D
	# y2
	MOV R14D, 0x2d0
	VCVTSI2SS XMM3,XMM3,R14D
	LEA R14,[RBP + -0x88]
	MOV RDI,R14
	LEA R14, [0x01918048]
	CMP dword ptr [R14],0xf # above 16 index, reset
	JA color_default
	CMP dword ptr [R14],0x0 # check color index
	JZ color_0
	CMP dword ptr [R14],0x1
	JZ color_1
	CMP dword ptr [R14],0x2
	JZ color_2
	CMP dword ptr [R14],0x3
	JZ color_3
	CMP dword ptr [R14],0x4
	JZ color_4
	CMP dword ptr [R14],0x5
	JZ color_5
	CMP dword ptr [R14],0x6
	JZ color_6
	CMP dword ptr [R14],0x7
	JZ color_7
	CMP dword ptr [R14],0x8
	JZ color_8
	CMP dword ptr [R14],0x9
	JZ color_9
	CMP dword ptr [R14],0xa
	JZ color_10
	CMP dword ptr [R14],0xb
	JZ color_11
	CMP dword ptr [R14],0xc
	JZ color_12
	CMP dword ptr [R14],0xd
	JZ color_13
	CMP dword ptr [R14],0xe
	JZ color_14
	CMP dword ptr [R14],0xf
	JZ color_15
	color_default:
	MOV ESI, 0xFFFFFFFF # first index color
	MOV dword ptr [R14],1 # next frame, use second index
	JMP draw_bar_only_no_add
	color_0:
	MOV ESI, 0xFFFFFFFF # White
	JMP draw_bar
	color_1:
	MOV ESI, 0xFF00FF00 # Lime (Green)
	JMP draw_bar
	color_2:
	MOV ESI, 0xFFFF0000 # Blue
	JMP draw_bar
	color_3:
	MOV ESI, 0xFF0000FF # Red
	JMP draw_bar
	color_4:
	MOV ESI, 0xFF808000 # Teal
	JMP draw_bar
	color_5:
	MOV ESI, 0xFF800000 # Navy
	JMP draw_bar
	color_6:
	MOV ESI, 0xFF008000 # Green
	JMP draw_bar
	color_7:
	MOV ESI, 0xFFFFFF00 # Aqua
	JMP draw_bar
	color_8:
	MOV ESI, 0xFF000080 # Dark Green? It's dark red
	JMP draw_bar
	color_9:
	MOV ESI, 0xFFC0C0C0 # Silver
	JMP draw_bar
	color_10:
	MOV ESI, 0xFF800080 # Purple
	JMP draw_bar
	color_11:
	MOV ESI, 0xFF008080 # Olive
	JMP draw_bar
	color_12:
	MOV ESI, 0xFF808080 # Gray
	JMP draw_bar
	color_13:
	MOV ESI, 0xFFFF00FF # Fuchsia
	JMP draw_bar
	color_14:
	MOV ESI, 0xFF00FFFF # Yellow
	JMP draw_bar
	color_15:
	MOV ESI, 0xFF0080FF # Orange
	# JMP draw_bar
	draw_bar:
	ADD dword ptr [R14],1
	draw_bar_only_no_add:
	CALL 0x00ec50e0
	exit_func:
	ADD RSP,0x78
	POP RBX
	POP R12
	POP R13
	POP R14
	POP R15
	POP RBP
	RET
	*/
	// <!-- Always Draw Paused Panel -->
	write_bytes(app_pid, NO_ASLR(0x00e31036), "9090");
	write_bytes(app_pid, NO_ASLR(0x00e3103d), "9090");
	write_bytes(app_pid, NO_ASLR(0x00e3104a), "9090");
	write_bytes(app_pid, NO_ASLR(0x00e31056), "9090");
	// <!-- Text -->
	write_string(app_pid, NO_ASLR(0x014f0942), "Draw Color Boarder");
	// <!-- Draw Bar -->
	write_bytes(app_pid, NO_ASLR(0x00e311d0), "55");
	write_bytes(app_pid, NO_ASLR(0x00e311d1), "4889e5");
	write_bytes(app_pid, NO_ASLR(0x00e311d4), "4157");
	write_bytes(app_pid, NO_ASLR(0x00e311d6), "4156");
	write_bytes(app_pid, NO_ASLR(0x00e311d8), "4155");
	write_bytes(app_pid, NO_ASLR(0x00e311da), "4154");
	write_bytes(app_pid, NO_ASLR(0x00e311dc), "53");
	write_bytes(app_pid, NO_ASLR(0x00e311dd), "4883ec78");
	write_bytes(app_pid, NO_ASLR(0x00e311e1), "488d3d8895de00");
	write_bytes(app_pid, NO_ASLR(0x00e311e8), "80bfc007000000");
	write_bytes(app_pid, NO_ASLR(0x00e311ef), "0f844b010000");
	write_bytes(app_pid, NO_ASLR(0x00e311f5), "488dbd78ffffff");
	write_bytes(app_pid, NO_ASLR(0x00e311fc), "be01000000");
	write_bytes(app_pid, NO_ASLR(0x00e31201), "31d2");
	write_bytes(app_pid, NO_ASLR(0x00e31203), "e888390900");
	write_bytes(app_pid, NO_ASLR(0x00e31208), "c5f857c0");
	write_bytes(app_pid, NO_ASLR(0x00e3120c), "c5f057c9");
	write_bytes(app_pid, NO_ASLR(0x00e31210), "c5e857d2");
	write_bytes(app_pid, NO_ASLR(0x00e31214), "c5e057db");
	write_bytes(app_pid, NO_ASLR(0x00e31218), "41be00000000");
	write_bytes(app_pid, NO_ASLR(0x00e3121e), "c4c17a2ac6");
	write_bytes(app_pid, NO_ASLR(0x00e31223), "41be00000000");
	write_bytes(app_pid, NO_ASLR(0x00e31229), "c4c1722ace");
	write_bytes(app_pid, NO_ASLR(0x00e3122e), "41be20000000");
	write_bytes(app_pid, NO_ASLR(0x00e31234), "c4c16a2ad6");
	write_bytes(app_pid, NO_ASLR(0x00e31239), "41bed0020000");
	write_bytes(app_pid, NO_ASLR(0x00e3123f), "c4c1622ade");
	write_bytes(app_pid, NO_ASLR(0x00e31244), "4c8db578ffffff");
	write_bytes(app_pid, NO_ASLR(0x00e3124b), "4c89f7");
	write_bytes(app_pid, NO_ASLR(0x00e3124e), "4c8d35f36dae00");
	write_bytes(app_pid, NO_ASLR(0x00e31255), "41833e0f");
	write_bytes(app_pid, NO_ASLR(0x00e31259), "7760");
	write_bytes(app_pid, NO_ASLR(0x00e3125b), "41833e00");
	write_bytes(app_pid, NO_ASLR(0x00e3125f), "7468");
	write_bytes(app_pid, NO_ASLR(0x00e31261), "41833e01");
	write_bytes(app_pid, NO_ASLR(0x00e31265), "7469");
	write_bytes(app_pid, NO_ASLR(0x00e31267), "41833e02");
	write_bytes(app_pid, NO_ASLR(0x00e3126b), "746a");
	write_bytes(app_pid, NO_ASLR(0x00e3126d), "41833e03");
	write_bytes(app_pid, NO_ASLR(0x00e31271), "746b");
	write_bytes(app_pid, NO_ASLR(0x00e31273), "41833e04");
	write_bytes(app_pid, NO_ASLR(0x00e31277), "746c");
	write_bytes(app_pid, NO_ASLR(0x00e31279), "41833e05");
	write_bytes(app_pid, NO_ASLR(0x00e3127d), "746d");
	write_bytes(app_pid, NO_ASLR(0x00e3127f), "41833e06");
	write_bytes(app_pid, NO_ASLR(0x00e31283), "746e");
	write_bytes(app_pid, NO_ASLR(0x00e31285), "41833e07");
	write_bytes(app_pid, NO_ASLR(0x00e31289), "746f");
	write_bytes(app_pid, NO_ASLR(0x00e3128b), "41833e08");
	write_bytes(app_pid, NO_ASLR(0x00e3128f), "7470");
	write_bytes(app_pid, NO_ASLR(0x00e31291), "41833e09");
	write_bytes(app_pid, NO_ASLR(0x00e31295), "7471");
	write_bytes(app_pid, NO_ASLR(0x00e31297), "41833e0a");
	write_bytes(app_pid, NO_ASLR(0x00e3129b), "7472");
	write_bytes(app_pid, NO_ASLR(0x00e3129d), "41833e0b");
	write_bytes(app_pid, NO_ASLR(0x00e312a1), "7473");
	write_bytes(app_pid, NO_ASLR(0x00e312a3), "41833e0c");
	write_bytes(app_pid, NO_ASLR(0x00e312a7), "7474");
	write_bytes(app_pid, NO_ASLR(0x00e312a9), "41833e0d");
	write_bytes(app_pid, NO_ASLR(0x00e312ad), "7475");
	write_bytes(app_pid, NO_ASLR(0x00e312af), "41833e0e");
	write_bytes(app_pid, NO_ASLR(0x00e312b3), "7476");
	write_bytes(app_pid, NO_ASLR(0x00e312b5), "41833e0f");
	write_bytes(app_pid, NO_ASLR(0x00e312b9), "7477");
	write_bytes(app_pid, NO_ASLR(0x00e312bb), "beffffffff");
	write_bytes(app_pid, NO_ASLR(0x00e312c0), "41c70601000000");
	write_bytes(app_pid, NO_ASLR(0x00e312c7), "eb72");
	write_bytes(app_pid, NO_ASLR(0x00e312c9), "beffffffff");
	write_bytes(app_pid, NO_ASLR(0x00e312ce), "eb67");
	write_bytes(app_pid, NO_ASLR(0x00e312d0), "be00ff00ff");
	write_bytes(app_pid, NO_ASLR(0x00e312d5), "eb60");
	write_bytes(app_pid, NO_ASLR(0x00e312d7), "be0000ffff");
	write_bytes(app_pid, NO_ASLR(0x00e312dc), "eb59");
	write_bytes(app_pid, NO_ASLR(0x00e312de), "beff0000ff");
	write_bytes(app_pid, NO_ASLR(0x00e312e3), "eb52");
	write_bytes(app_pid, NO_ASLR(0x00e312e5), "be008080ff");
	write_bytes(app_pid, NO_ASLR(0x00e312ea), "eb4b");
	write_bytes(app_pid, NO_ASLR(0x00e312ec), "be000080ff");
	write_bytes(app_pid, NO_ASLR(0x00e312f1), "eb44");
	write_bytes(app_pid, NO_ASLR(0x00e312f3), "be008000ff");
	write_bytes(app_pid, NO_ASLR(0x00e312f8), "eb3d");
	write_bytes(app_pid, NO_ASLR(0x00e312fa), "be00ffffff");
	write_bytes(app_pid, NO_ASLR(0x00e312ff), "eb36");
	write_bytes(app_pid, NO_ASLR(0x00e31301), "be800000ff");
	write_bytes(app_pid, NO_ASLR(0x00e31306), "eb2f");
	write_bytes(app_pid, NO_ASLR(0x00e31308), "bec0c0c0ff");
	write_bytes(app_pid, NO_ASLR(0x00e3130d), "eb28");
	write_bytes(app_pid, NO_ASLR(0x00e3130f), "be800080ff");
	write_bytes(app_pid, NO_ASLR(0x00e31314), "eb21");
	write_bytes(app_pid, NO_ASLR(0x00e31316), "be808000ff");
	write_bytes(app_pid, NO_ASLR(0x00e3131b), "eb1a");
	write_bytes(app_pid, NO_ASLR(0x00e3131d), "be808080ff");
	write_bytes(app_pid, NO_ASLR(0x00e31322), "eb13");
	write_bytes(app_pid, NO_ASLR(0x00e31324), "beff00ffff");
	write_bytes(app_pid, NO_ASLR(0x00e31329), "eb0c");
	write_bytes(app_pid, NO_ASLR(0x00e3132b), "beffff00ff");
	write_bytes(app_pid, NO_ASLR(0x00e31330), "eb05");
	write_bytes(app_pid, NO_ASLR(0x00e31332), "beff8000ff");
	write_bytes(app_pid, NO_ASLR(0x00e31337), "41830601");
	write_bytes(app_pid, NO_ASLR(0x00e3133b), "e8a03d0900");
	write_bytes(app_pid, NO_ASLR(0x00e31340), "4883c478");
	write_bytes(app_pid, NO_ASLR(0x00e31344), "5b");
	write_bytes(app_pid, NO_ASLR(0x00e31345), "415c");
	write_bytes(app_pid, NO_ASLR(0x00e31347), "415d");
	write_bytes(app_pid, NO_ASLR(0x00e31349), "415e");
	write_bytes(app_pid, NO_ASLR(0x00e3134b), "415f");
	write_bytes(app_pid, NO_ASLR(0x00e3134d), "5d");
	write_bytes(app_pid, NO_ASLR(0x00e3134e), "c3");
	// 120Hz
	// if (g_isPatch120Hz)
	{
		write_bytes(app_pid, NO_ASLR(0x00f3d26b), "e9c06a9300");
		write_bytes(app_pid, NO_ASLR(0x01873d30), "e8eb33c1ff");
		write_bytes(app_pid, NO_ASLR(0x01873d35), "488d3de4450c00");
		write_bytes(app_pid, NO_ASLR(0x01873d3c), "8907");
		write_bytes(app_pid, NO_ASLR(0x01873d3e), "488d056bff0900");
		write_bytes(app_pid, NO_ASLR(0x01873d45), "488b00");
		write_bytes(app_pid, NO_ASLR(0x01873d48), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x01873d4e), "8b3d5c505c00");
		write_bytes(app_pid, NO_ASLR(0x01873d54), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x01873d59), "31d2");
		write_bytes(app_pid, NO_ASLR(0x01873d5b), "31c9");
		write_bytes(app_pid, NO_ASLR(0x01873d5d), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x01873d60), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x01873d62), "83f801");
		write_bytes(app_pid, NO_ASLR(0x01873d65), "7524");
		write_bytes(app_pid, NO_ASLR(0x01873d67), "488d0542ff0900");
		write_bytes(app_pid, NO_ASLR(0x01873d6e), "488b00");
		write_bytes(app_pid, NO_ASLR(0x01873d71), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x01873d77), "8b3d33505c00");
		write_bytes(app_pid, NO_ASLR(0x01873d7d), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x01873d82), "31d2");
		write_bytes(app_pid, NO_ASLR(0x01873d84), "31c9");
		write_bytes(app_pid, NO_ASLR(0x01873d86), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x01873d89), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x01873d8b), "488d058e450c00");
		write_bytes(app_pid, NO_ASLR(0x01873d92), "8b00");
		write_bytes(app_pid, NO_ASLR(0x01873d94), "e9dd946cff");
		// timestep fixes
		write_float32(app_pid, NO_ASLR(0x0158f4b4), (1. / 120.));
		write_float32(app_pid, NO_ASLR(0x0158f4b8), (1. / 120.));
		write_float32(app_pid, NO_ASLR(0x0158f4bc), 120.);
		printf_notification("Applied 120Hz Patch");
	}
}

void DoPatch_t2ps4(pid_t app_pid, uint64_t text_base, uint32_t idx)
{
	switch (idx)
	{
	case 0x100:
	{
		// Debug Menu
		write_bytes(app_pid, NO_ASLR(0x005d239c), "b001");
		// 60 FPS
		write_bytes(app_pid, NO_ASLR(0x02a9e570), "31c0c3");
		// Skip Startup Logos
		write_bytes(app_pid, NO_ASLR(0x01c44b0b), "48e8a2cbe500");
		write_bytes(app_pid, NO_ASLR(0x02aa16b3), "817b2000004040");
		write_bytes(app_pid, NO_ASLR(0x02aa16ba), "7514");
		write_bytes(app_pid, NO_ASLR(0x02aa16bc), "817b780000e040");
		write_bytes(app_pid, NO_ASLR(0x02aa16c3), "750b");
		write_bytes(app_pid, NO_ASLR(0x02aa16c5), "4531f6");
		write_bytes(app_pid, NO_ASLR(0x02aa16c8), "44897320");
		write_bytes(app_pid, NO_ASLR(0x02aa16cc), "44897378");
		write_bytes(app_pid, NO_ASLR(0x02aa16d0), "440fb6748301");
		write_bytes(app_pid, NO_ASLR(0x02aa16d6), "c3");
		break;
	}
	case 0x109:
	{
		// Debug Menu
		write_bytes(app_pid, NO_ASLR(0x00a141dd), "b000");
		// write_float32(app_pid, NO_ASLR(0x00ba0ecf), 1.0f);
		// 60 FPS
		write_bytes(app_pid, NO_ASLR(0x02bbb5f0), "31f6");
		write_bytes(app_pid, NO_ASLR(0x02bbb5f2), "ff25e0e17e00");
		// Skip Startup Logos
		write_bytes(app_pid, NO_ASLR(0x01c85f20), "c3");
		write_bytes(app_pid, NO_ASLR(0x020b1952), "48e8f85cb200");
		write_bytes(app_pid, NO_ASLR(0x02bd7650), "817a2000004040");
		write_bytes(app_pid, NO_ASLR(0x02bd7657), "7514");
		write_bytes(app_pid, NO_ASLR(0x02bd7659), "817a780000e040");
		write_bytes(app_pid, NO_ASLR(0x02bd7660), "750b");
		write_bytes(app_pid, NO_ASLR(0x02bd7662), "4531c9");
		write_bytes(app_pid, NO_ASLR(0x02bd7665), "44894a20");
		write_bytes(app_pid, NO_ASLR(0x02bd7669), "44894a78");
		write_bytes(app_pid, NO_ASLR(0x02bd766d), "440fb6448201");
		write_bytes(app_pid, NO_ASLR(0x02bd7673), "c3");
		// Give Player Weapon Names Fix (Partial)
		/*
		write_bytes(app_pid, NO_ASLR(0x0174ef88), "48e823c02b01");
		write_bytes(app_pid, NO_ASLR(0x02a0afb1), "8b4f60");
		write_bytes(app_pid, NO_ASLR(0x02a0afb4), "48891d75509f00");
		write_bytes(app_pid, NO_ASLR(0x02a0afbb), "4889158e509f00");
		write_bytes(app_pid, NO_ASLR(0x02a0afc2), "4c890d8f509f00");
		write_bytes(app_pid, NO_ASLR(0x02a0afc9), "4c8d0d80010000");
		write_bytes(app_pid, NO_ASLR(0x02a0afd0), "488d1d51509f00");
		write_bytes(app_pid, NO_ASLR(0x02a0afd7), "b800eea388");
		write_bytes(app_pid, NO_ASLR(0x02a0afdc), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0afde), "0f8488000000");
		write_bytes(app_pid, NO_ASLR(0x02a0afe4), "b8210de0b2");
		write_bytes(app_pid, NO_ASLR(0x02a0afe9), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0afeb), "0f8490000000");
		write_bytes(app_pid, NO_ASLR(0x02a0aff1), "b88c0a3298");
		write_bytes(app_pid, NO_ASLR(0x02a0aff6), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0aff8), "0f8498000000");
		write_bytes(app_pid, NO_ASLR(0x02a0affe), "b8dd229b6a");
		write_bytes(app_pid, NO_ASLR(0x02a0b003), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b005), "0f84a0000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b00b), "b8c60cdf31");
		write_bytes(app_pid, NO_ASLR(0x02a0b010), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b012), "0f84a8000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b018), "b8d9951c4a");
		write_bytes(app_pid, NO_ASLR(0x02a0b01d), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b01f), "0f84b0000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b025), "b80a656f99");
		write_bytes(app_pid, NO_ASLR(0x02a0b02a), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b02c), "0f84b8000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b032), "90");
		write_bytes(app_pid, NO_ASLR(0x02a0b033), "b8a20ff751");
		write_bytes(app_pid, NO_ASLR(0x02a0b038), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b03a), "0f84bf000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b040), "b875f44c8e");
		write_bytes(app_pid, NO_ASLR(0x02a0b045), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b047), "0f84c7000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b04d), "b8e2683441");
		write_bytes(app_pid, NO_ASLR(0x02a0b052), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b054), "0f84cf000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b05a), "b8cdcb0831");
		write_bytes(app_pid, NO_ASLR(0x02a0b05f), "39c1");
		write_bytes(app_pid, NO_ASLR(0x02a0b061), "0f84d7000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b067), "e9e4000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b06c), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b06f), "488d152e120000");
		write_bytes(app_pid, NO_ASLR(0x02a0b076), "c6031f");
		write_bytes(app_pid, NO_ASLR(0x02a0b079), "e8ed000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b07e), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b081), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b084), "488d15b9010000");
		write_bytes(app_pid, NO_ASLR(0x02a0b08b), "c60320");
		write_bytes(app_pid, NO_ASLR(0x02a0b08e), "e8d8000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b093), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b096), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b099), "488d1584060000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0a0), "c60321");
		write_bytes(app_pid, NO_ASLR(0x02a0b0a3), "e8c3000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0a8), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b0ab), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b0ae), "488d15df070000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0b5), "c60322");
		write_bytes(app_pid, NO_ASLR(0x02a0b0b8), "e8ae000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0bd), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b0c0), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b0c3), "488d151a090000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0ca), "c60323");
		write_bytes(app_pid, NO_ASLR(0x02a0b0cd), "e899000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0d2), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b0d5), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b0d8), "488d15050c0000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0df), "c60324");
		write_bytes(app_pid, NO_ASLR(0x02a0b0e2), "e884000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0e7), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b0ea), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b0ed), "488d15900d0000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0f4), "c60325");
		write_bytes(app_pid, NO_ASLR(0x02a0b0f7), "e86f000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b0fc), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b0ff), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b102), "488d154b0e0000");
		write_bytes(app_pid, NO_ASLR(0x02a0b109), "c60326");
		write_bytes(app_pid, NO_ASLR(0x02a0b10c), "e85a000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b111), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b114), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b117), "488d15360f0000");
		write_bytes(app_pid, NO_ASLR(0x02a0b11e), "c60327");
		write_bytes(app_pid, NO_ASLR(0x02a0b121), "e845000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b126), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b129), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b12c), "488d15f10f0000");
		write_bytes(app_pid, NO_ASLR(0x02a0b133), "c60328");
		write_bytes(app_pid, NO_ASLR(0x02a0b136), "e830000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b13b), "41ffe1");
		write_bytes(app_pid, NO_ASLR(0x02a0b13e), "41b428");
		write_bytes(app_pid, NO_ASLR(0x02a0b141), "488d15dc100000");
		write_bytes(app_pid, NO_ASLR(0x02a0b148), "c60329");
		write_bytes(app_pid, NO_ASLR(0x02a0b14b), "e81b000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b150), "e826000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b155), "488b1dd44e9f00");
		write_bytes(app_pid, NO_ASLR(0x02a0b15c), "488b15ed4e9f00");
		write_bytes(app_pid, NO_ASLR(0x02a0b163), "4c8b0dee4e9f00");
		write_bytes(app_pid, NO_ASLR(0x02a0b16a), "c3");
		write_bytes(app_pid, NO_ASLR(0x02a0b16b), "48c7435800000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b173), "48895350");
		write_bytes(app_pid, NO_ASLR(0x02a0b177), "c3");
		write_bytes(app_pid, NO_ASLR(0x02a0b17b), "48893dee4e9f00");
		write_bytes(app_pid, NO_ASLR(0x02a0b182), "488b5350");
		write_bytes(app_pid, NO_ASLR(0x02a0b186), "4883435828");
		write_bytes(app_pid, NO_ASLR(0x02a0b18b), "48035358");
		write_bytes(app_pid, NO_ASLR(0x02a0b18f), "803a30");
		write_bytes(app_pid, NO_ASLR(0x02a0b192), "7410");
		write_bytes(app_pid, NO_ASLR(0x02a0b194), "807f2f01");
		write_bytes(app_pid, NO_ASLR(0x02a0b198), "7412");
		write_bytes(app_pid, NO_ASLR(0x02a0b19a), "48895730");
		write_bytes(app_pid, NO_ASLR(0x02a0b19e), "c6472f01");
		write_bytes(app_pid, NO_ASLR(0x02a0b1a2), "eb08");
		write_bytes(app_pid, NO_ASLR(0x02a0b1a4), "48c7435800000000");
		write_bytes(app_pid, NO_ASLR(0x02a0b1ac), "48893dbd4e9f00");
		write_bytes(app_pid, NO_ASLR(0x02a0b1b3), "41b401");
		write_bytes(app_pid, NO_ASLR(0x02a0b1b6), "83fe05");
		write_bytes(app_pid, NO_ASLR(0x02a0b1b9), "c3");
		write_string(app_pid, NO_ASLR(0x02a0b26c), "hammer-modern                          ");
		write_string(app_pid, NO_ASLR(0x02a0b294), "hammer-scar                            ");
		write_string(app_pid, NO_ASLR(0x02a0b2bc), "crowbar-one-hand                       ");
		write_string(app_pid, NO_ASLR(0x02a0b2e4), "pipe-wrench                            ");
		write_string(app_pid, NO_ASLR(0x02a0b30c), "club-1h-scar                           ");
		write_string(app_pid, NO_ASLR(0x02a0b334), "club-1h-police                         ");
		write_string(app_pid, NO_ASLR(0x02a0b35c), "machete-black                          ");
		write_string(app_pid, NO_ASLR(0x02a0b384), "machete-wood                           ");
		write_string(app_pid, NO_ASLR(0x02a0b3ac), "machete-scar                           ");
		write_string(app_pid, NO_ASLR(0x02a0b3d4), "hatchet-modern                         ");
		write_string(app_pid, NO_ASLR(0x02a0b3fc), "hatchet-black                          ");
		write_string(app_pid, NO_ASLR(0x02a0b424), "hatchet-scar                           ");
		write_string(app_pid, NO_ASLR(0x02a0b44c), "scythe-scar                            ");
		write_string(app_pid, NO_ASLR(0x02a0b474), "hatchet-tomahawk                       ");
		write_string(app_pid, NO_ASLR(0x02a0b49c), "baseball-bat                           ");
		write_string(app_pid, NO_ASLR(0x02a0b4c4), "baseball-bat-metal                     ");
		write_string(app_pid, NO_ASLR(0x02a0b4ec), "two-by-four                            ");
		write_string(app_pid, NO_ASLR(0x02a0b514), "lead-pipe                              ");
		write_string(app_pid, NO_ASLR(0x02a0b53c), "club-2h-scar                           ");
		write_string(app_pid, NO_ASLR(0x02a0b564), "two-hand-axe                           ");
		write_string(app_pid, NO_ASLR(0x02a0b58c), "two-hand-axe-scar                      ");
		write_string(app_pid, NO_ASLR(0x02a0b5b4), "fireman-axe                            ");
		write_string(app_pid, NO_ASLR(0x02a0b5dc), "switchblade                            ");
		write_string(app_pid, NO_ASLR(0x02a0b604), "switchblade                            ");
		write_string(app_pid, NO_ASLR(0x02a0b62c), "switchblade                            ");
		write_string(app_pid, NO_ASLR(0x02a0b654), "switchblade                            ");
		write_string(app_pid, NO_ASLR(0x02a0b67c), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0b6a4), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0b6cc), "switchblade                            ");
		write_string(app_pid, NO_ASLR(0x02a0b6f4), "pickaxe-scar                           ");
		write_string(app_pid, NO_ASLR(0x02a0b71c), "pickaxe-scar-player                    ");
		write_string(app_pid, NO_ASLR(0x02a0b744), "0");
		write_string(app_pid, NO_ASLR(0x02a0b74c), "pistol-beretta                         ");
		write_string(app_pid, NO_ASLR(0x02a0b774), "pistol-revolver-taurus                 ");
		write_string(app_pid, NO_ASLR(0x02a0b79c), "bow-ellie                              ");
		write_string(app_pid, NO_ASLR(0x02a0b7c4), "shotgun-remington-pump                 ");
		write_string(app_pid, NO_ASLR(0x02a0b7ec), "rifle-remington-bolt                   ");
		write_string(app_pid, NO_ASLR(0x02a0b814), "rifle-galil                            ");
		write_string(app_pid, NO_ASLR(0x02a0b83c), "rifle-mpx5                             ");
		write_string(app_pid, NO_ASLR(0x02a0b864), "rifle-remington-81                     ");
		write_string(app_pid, NO_ASLR(0x02a0b88c), "0");
		write_string(app_pid, NO_ASLR(0x02a0b8bc), "pistol-glock                           ");
		write_string(app_pid, NO_ASLR(0x02a0b8e4), "shotgun-baserri-over-under             ");
		write_string(app_pid, NO_ASLR(0x02a0b90c), "shotgun-dbl-barrel-abby                ");
		write_string(app_pid, NO_ASLR(0x02a0b934), "crossbow                               ");
		write_string(app_pid, NO_ASLR(0x02a0b95c), "rifle-vepr                             ");
		write_string(app_pid, NO_ASLR(0x02a0b984), "pistol-xcaliber                        ");
		write_string(app_pid, NO_ASLR(0x02a0b9ac), "flamethrower                           ");
		write_string(app_pid, NO_ASLR(0x02a0b9d4), "pistol-revolver-taurus-owen            ");
		write_string(app_pid, NO_ASLR(0x02a0b9fc), "0");
		write_string(app_pid, NO_ASLR(0x02a0ba0c), "                                       ");
		write_string(app_pid, NO_ASLR(0x02a0ba34), "pistol-ruger                           ");
		write_string(app_pid, NO_ASLR(0x02a0ba5c), "pistol-ruger                           ");
		write_string(app_pid, NO_ASLR(0x02a0ba84), "pistol-browning-jesse                  ");
		write_string(app_pid, NO_ASLR(0x02a0baac), "pistol-browning                        ");
		write_string(app_pid, NO_ASLR(0x02a0bad4), "pistol-colt-1911-silver                ");
		write_string(app_pid, NO_ASLR(0x02a0bafc), "pistol-beretta-black                   ");
		write_string(app_pid, NO_ASLR(0x02a0bb24), "pistol-revolver-taurus                 ");
		write_string(app_pid, NO_ASLR(0x02a0bb4c), "pistol-revolver-taurus-owen            ");
		write_string(app_pid, NO_ASLR(0x02a0bb74), "shotgun-remington-pump-sawedoff        ");
		write_string(app_pid, NO_ASLR(0x02a0bb9c), "shotgun-pump-stock                     ");
		write_string(app_pid, NO_ASLR(0x02a0bbc4), "rifle-marlin-lever                     ");
		write_string(app_pid, NO_ASLR(0x02a0bbec), "rifle-winchester-lever                 ");
		write_string(app_pid, NO_ASLR(0x02a0bc14), "rifle-remington-81                     ");
		write_string(app_pid, NO_ASLR(0x02a0bc3c), "rifle-mini-14-manny                    ");
		write_string(app_pid, NO_ASLR(0x02a0bc64), "rifle-mini-14-wood                     ");
		write_string(app_pid, NO_ASLR(0x02a0bc8c), "rifle-m14-sniper                       ");
		write_string(app_pid, NO_ASLR(0x02a0bcb4), "bow-lev                                ");
		write_string(app_pid, NO_ASLR(0x02a0bcdc), "bow-joel                               ");
		write_string(app_pid, NO_ASLR(0x02a0bd04), "0");
		write_string(app_pid, NO_ASLR(0x02a0bd0c), "molotov                                ");
		write_string(app_pid, NO_ASLR(0x02a0bd34), "pipe-bomb                              ");
		write_string(app_pid, NO_ASLR(0x02a0bd5c), "beer-bottle                            ");
		write_string(app_pid, NO_ASLR(0x02a0bd84), "throw-brick                            ");
		write_string(app_pid, NO_ASLR(0x02a0bdac), "stun-bomb                              ");
		write_string(app_pid, NO_ASLR(0x02a0bdd4), "smoke-bomb                             ");
		write_string(app_pid, NO_ASLR(0x02a0bdfc), "pat-snowball                           ");
		write_string(app_pid, NO_ASLR(0x02a0be24), "trap-mine                              ");
		write_string(app_pid, NO_ASLR(0x02a0be4c), "molotov                                ");
		write_string(app_pid, NO_ASLR(0x02a0be74), "ski-aquarium-dog-toy                   ");
		write_string(app_pid, NO_ASLR(0x02a0be9c), "0");
		write_string(app_pid, NO_ASLR(0x02a0beac), "bandage-apply                          ");
		write_string(app_pid, NO_ASLR(0x02a0bed4), "bandage-apply                          ");
		write_string(app_pid, NO_ASLR(0x02a0befc), "syringe                                ");
		write_string(app_pid, NO_ASLR(0x02a0bf24), "                                       ");
		write_string(app_pid, NO_ASLR(0x02a0bf4c), "                                       ");
		write_string(app_pid, NO_ASLR(0x02a0bf74), "0");
		write_string(app_pid, NO_ASLR(0x02a0bf7c), "pistol-ruger                           ");
		write_string(app_pid, NO_ASLR(0x02a0bfa4), "pistol-revolver-militia                ");
		write_string(app_pid, NO_ASLR(0x02a0bfcc), "rifle-mini-14                          ");
		write_string(app_pid, NO_ASLR(0x02a0bff4), "rifle-bolt-m24                         ");
		write_string(app_pid, NO_ASLR(0x02a0c01c), "shotgun-remington-pump-black           ");
		write_string(app_pid, NO_ASLR(0x02a0c044), "                                       ");
		write_string(app_pid, NO_ASLR(0x02a0c06c), "0");
		write_string(app_pid, NO_ASLR(0x02a0c07c), "shotgun-dbl-barrel-scar                ");
		write_string(app_pid, NO_ASLR(0x02a0c0a4), "pistol-browning-scar                   ");
		write_string(app_pid, NO_ASLR(0x02a0c0cc), "pistol-revolver-scar                   ");
		write_string(app_pid, NO_ASLR(0x02a0c0f4), "bow-scar                               ");
		write_string(app_pid, NO_ASLR(0x02a0c11c), "rifle-bolt-scar                        ");
		write_string(app_pid, NO_ASLR(0x02a0c144), "0");
		write_string(app_pid, NO_ASLR(0x02a0c14c), "pistol-cz75-black                      ");
		write_string(app_pid, NO_ASLR(0x02a0c174), "pistol-revolver-357                    ");
		write_string(app_pid, NO_ASLR(0x02a0c19c), "rifle-m14-dark                         ");
		write_string(app_pid, NO_ASLR(0x02a0c1c4), "rifle-bolt-m24-dark                    ");
		write_string(app_pid, NO_ASLR(0x02a0c1ec), "shotgun-pump-stock-black               ");
		write_string(app_pid, NO_ASLR(0x02a0c214), "rifle-mpx5-tilt                        ");
		write_string(app_pid, NO_ASLR(0x02a0c23c), "0");
		write_string(app_pid, NO_ASLR(0x02a0c24c), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c274), "                                       ");
		write_string(app_pid, NO_ASLR(0x02a0c29c), "bandage-apply                          ");
		write_string(app_pid, NO_ASLR(0x02a0c2c4), "0");
		write_string(app_pid, NO_ASLR(0x02a0c2cc), "knife-survival-mel                     ");
		write_string(app_pid, NO_ASLR(0x02a0c2f4), "bloater-pustule                        ");
		write_string(app_pid, NO_ASLR(0x02a0c31c), "pistol-colt-1911-dark                  ");
		write_string(app_pid, NO_ASLR(0x02a0c344), "knife-hunting                          ");
		write_string(app_pid, NO_ASLR(0x02a0c36c), "shiv-d                                 ");
		write_string(app_pid, NO_ASLR(0x02a0c394), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c3bc), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c3e4), "rifle-m14-sniper                       ");
		write_string(app_pid, NO_ASLR(0x02a0c40c), "bow-workbench                          ");
		write_string(app_pid, NO_ASLR(0x02a0c434), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c45c), "bow-ellie                              ");
		write_string(app_pid, NO_ASLR(0x02a0c484), "rifle-m14-sniper                       ");
		write_string(app_pid, NO_ASLR(0x02a0c4ac), "bow-scar                               ");
		write_string(app_pid, NO_ASLR(0x02a0c4d4), "molotov                                ");
		write_string(app_pid, NO_ASLR(0x02a0c4fc), "pipe-bomb                              ");
		write_string(app_pid, NO_ASLR(0x02a0c524), "gas-mask-lev                           ");
		write_string(app_pid, NO_ASLR(0x02a0c54c), "rifle-m14-sniper                       ");
		write_string(app_pid, NO_ASLR(0x02a0c574), "shotgun-pump-stock                     ");
		write_string(app_pid, NO_ASLR(0x02a0c59c), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c5c4), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c5ec), "shiv-b                                 ");
		write_string(app_pid, NO_ASLR(0x02a0c614), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c63c), "hatchet-tomahawk                       ");
		write_string(app_pid, NO_ASLR(0x02a0c664), "short-pipe                             ");
		write_string(app_pid, NO_ASLR(0x02a0c68c), "switchblade                            ");
		write_string(app_pid, NO_ASLR(0x02a0c6b4), "rifle-m14-sniper                       ");
		write_string(app_pid, NO_ASLR(0x02a0c6dc), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c704), "torch-game                             ");
		write_string(app_pid, NO_ASLR(0x02a0c72c), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c754), "molotov                                ");
		write_string(app_pid, NO_ASLR(0x02a0c77c), "machete-wood-joel                      ");
		write_string(app_pid, NO_ASLR(0x02a0c7a4), "knife-hunting                          ");
		write_string(app_pid, NO_ASLR(0x02a0c7cc), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c7f4), "shiv-f                                 ");
		write_string(app_pid, NO_ASLR(0x02a0c81c), "shiv-a                                 ");
		write_string(app_pid, NO_ASLR(0x02a0c844), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c86c), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c894), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c8bc), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0c8e4), "fob-basketball                         ");
		write_string(app_pid, NO_ASLR(0x02a0c90c), "bloater-pustule                        ");
		write_string(app_pid, NO_ASLR(0x02a0c934), "rifle-ruger-scar                       ");
		write_string(app_pid, NO_ASLR(0x02a0c95c), "shotgun-remington-pump-sawedoff        ");
		write_string(app_pid, NO_ASLR(0x02a0c984), "amp-gift-shop-toy-orca                 ");
		write_string(app_pid, NO_ASLR(0x02a0c9ac), "machete-black                          ");
		write_string(app_pid, NO_ASLR(0x02a0c9d4), "bloater-pustule                        ");
		write_string(app_pid, NO_ASLR(0x02a0c9fc), "bloater-pustule                        ");
		write_string(app_pid, NO_ASLR(0x02a0ca24), "bow-toy                                ");
		write_string(app_pid, NO_ASLR(0x02a0ca4c), "shiv-e                                 ");
		write_string(app_pid, NO_ASLR(0x02a0ca74), "dagger-scar-lev                        ");
		write_string(app_pid, NO_ASLR(0x02a0ca9c), "invisible-cube                         ");
		write_string(app_pid, NO_ASLR(0x02a0cac4), "firecracker                            ");
		write_string(app_pid, NO_ASLR(0x02a0caec), "shotgun-remington-pump                 ");
		write_string(app_pid, NO_ASLR(0x02a0cb14), "short-pipe                             ");
		write_string(app_pid, NO_ASLR(0x02a0cb3c), "dagger-scar                            ");
		write_string(app_pid, NO_ASLR(0x02a0cb64), "NO_ART_GROUP                           ");
		write_string(app_pid, NO_ASLR(0x02a0cb8c), "rifle-bolt-m24                         ");
		write_string(app_pid, NO_ASLR(0x02a0cbb4), "sledgehammer                           ");
		write_string(app_pid, NO_ASLR(0x02a0cbdc), "shiv-c                                 ");
		write_string(app_pid, NO_ASLR(0x02a0cc04), "pistol-cz75-dina                       ");
		write_string(app_pid, NO_ASLR(0x02a0cc2c), "tennis-ball-throw                      ");
		write_string(app_pid, NO_ASLR(0x02a0cc54), "0");
		// Print bow aiming stats (debug)
		write_bytes(app_pid, NO_ASLR(0x026c492a), "c5fa10152667d700");
		write_bytes(app_pid, NO_ASLR(0x00a6dad3), "488d3c2580014003");
		write_bytes(app_pid, NO_ASLR(0x00a79897), "e904db1802");
		write_bytes(app_pid, NO_ASLR(0x02c073a0), "e84bd5abff");
		write_bytes(app_pid, NO_ASLR(0x02c073a5), "e814000000");
		write_bytes(app_pid, NO_ASLR(0x02c073aa), "e8b3000000");
		write_bytes(app_pid, NO_ASLR(0x02c073af), "e8fd000000");
		write_bytes(app_pid, NO_ASLR(0x02c073b4), "e834010000");
		write_bytes(app_pid, NO_ASLR(0x02c073b9), "e9de24e7fd");
		write_bytes(app_pid, NO_ASLR(0x02c073be), "803d23c6db0000");
		write_bytes(app_pid, NO_ASLR(0x02c073c5), "0f8496000000");
		write_bytes(app_pid, NO_ASLR(0x02c073cb), "55");
		write_bytes(app_pid, NO_ASLR(0x02c073cc), "4889e5");
		write_bytes(app_pid, NO_ASLR(0x02c073cf), "50");
		write_bytes(app_pid, NO_ASLR(0x02c073d0), "488d05a98c7f00");
		write_bytes(app_pid, NO_ASLR(0x02c073d7), "c5fa104004");
		write_bytes(app_pid, NO_ASLR(0x02c073dc), "c5fa1008");
		write_bytes(app_pid, NO_ASLR(0x02c073e0), "c5fa10500c");
		write_bytes(app_pid, NO_ASLR(0x02c073e5), "c5fa105824");
		write_bytes(app_pid, NO_ASLR(0x02c073ea), "0fb6481f");
		write_bytes(app_pid, NO_ASLR(0x02c073ee), "be28010000");
		write_bytes(app_pid, NO_ASLR(0x02c073f3), "488d3dc68c7f00");
		write_bytes(app_pid, NO_ASLR(0x02c073fa), "488d150143ffff");
		write_bytes(app_pid, NO_ASLR(0x02c07401), "c5fa5ac0");
		write_bytes(app_pid, NO_ASLR(0x02c07405), "c5f25ac9");
		write_bytes(app_pid, NO_ASLR(0x02c07409), "c5ea5ad2");
		write_bytes(app_pid, NO_ASLR(0x02c0740d), "c5e25adb");
		write_bytes(app_pid, NO_ASLR(0x02c07411), "58");
		write_bytes(app_pid, NO_ASLR(0x02c07412), "e8b93efbff");
		write_bytes(app_pid, NO_ASLR(0x02c07417), "c5fa1015393c8300");
		write_bytes(app_pid, NO_ASLR(0x02c0741f), "c5fa1005e5b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c07427), "c5fa100de1b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c0742f), "488d154d04fdff");
		write_bytes(app_pid, NO_ASLR(0x02c07436), "488d0d838c7f00");
		write_bytes(app_pid, NO_ASLR(0x02c0743d), "beffffffff");
		write_bytes(app_pid, NO_ASLR(0x02c07442), "b004");
		write_bytes(app_pid, NO_ASLR(0x02c07444), "c5f828da");
		write_bytes(app_pid, NO_ASLR(0x02c07448), "e8c3c777ff");
		write_bytes(app_pid, NO_ASLR(0x02c0744d), "488d3d6c8c7f00");
		write_bytes(app_pid, NO_ASLR(0x02c07454), "ba28010000");
		write_bytes(app_pid, NO_ASLR(0x02c07459), "31f6");
		write_bytes(app_pid, NO_ASLR(0x02c0745b), "e8a062fbff");
		write_bytes(app_pid, NO_ASLR(0x02c07460), "5d");
		write_bytes(app_pid, NO_ASLR(0x02c07461), "c3");
		write_bytes(app_pid, NO_ASLR(0x02c07462), "55");
		write_bytes(app_pid, NO_ASLR(0x02c07463), "4889e5");
		write_bytes(app_pid, NO_ASLR(0x02c07466), "c5fa1015ea3b8300");
		write_bytes(app_pid, NO_ASLR(0x02c0746e), "c5fa100596b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c07476), "c5fa100d92b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c0747e), "488d1566682900");
		write_bytes(app_pid, NO_ASLR(0x02c07485), "488d0d2e0b5c01");
		write_bytes(app_pid, NO_ASLR(0x02c0748c), "beffffffff");
		write_bytes(app_pid, NO_ASLR(0x02c07491), "b004");
		write_bytes(app_pid, NO_ASLR(0x02c07493), "c5f828da");
		write_bytes(app_pid, NO_ASLR(0x02c07497), "e874c777ff");
		write_bytes(app_pid, NO_ASLR(0x02c0749c), "488d3d170b5c01");
		write_bytes(app_pid, NO_ASLR(0x02c074a3), "ba00100000");
		write_bytes(app_pid, NO_ASLR(0x02c074a8), "31f6");
		write_bytes(app_pid, NO_ASLR(0x02c074aa), "e85162fbff");
		write_bytes(app_pid, NO_ASLR(0x02c074af), "5d");
		write_bytes(app_pid, NO_ASLR(0x02c074b0), "c3");
		write_bytes(app_pid, NO_ASLR(0x02c074b1), "55");
		write_bytes(app_pid, NO_ASLR(0x02c074b2), "4889e5");
		write_bytes(app_pid, NO_ASLR(0x02c074b5), "c5fa101557b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c074bd), "c5fa100547b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c074c5), "c5fa100d43b31b00");
		write_bytes(app_pid, NO_ASLR(0x02c074cd), "488d15c8feffff");
		write_bytes(app_pid, NO_ASLR(0x02c074d4), "488d0dbd8c7f00");
		write_bytes(app_pid, NO_ASLR(0x02c074db), "bee0e0e0c0");
		write_bytes(app_pid, NO_ASLR(0x02c074e0), "b004");
		write_bytes(app_pid, NO_ASLR(0x02c074e2), "c5f828da");
		write_bytes(app_pid, NO_ASLR(0x02c074e6), "e825c777ff");
		write_bytes(app_pid, NO_ASLR(0x02c074eb), "5d");
		write_bytes(app_pid, NO_ASLR(0x02c074ec), "c3");
		write_bytes(app_pid, NO_ASLR(0x02c074ed), "c3");
		write_string(app_pid, NO_ASLR(0x02bfb702), "Current Charge Time: %.2f\nMax Charge Time: %.2f\nCurrent Sway Multiplier: %.2f\nCooldown Timer: %.2f\nUsing Bow?: %i\n");
		*/
		// Draw Color Boarder
		/*
		<!--
		// Code to generate hex color
		#include <stdint.h>
		#include <stdio.h>

		#define _rgba(r,g,b,a) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | r)
		#define rgba(r,g,b) _rgba(r,g,b,255)

		int32_t main() {
			// https://github.com/flightlessmango/MangoHud/blob/41f923b0b2be07f035df221d01625e11f1305fde/src/fcat.h#L20
			// const std::array<const ImColor,16> sequence={{{255, 255, 255},{0, 255, 0},{0, 0, 255},{255, 0, 0},{0, 128, 128},{0, 0, 128},{0, 128, 0},{0, 255, 255},{128, 0, 0},{192, 192, 192},{128, 0, 128},{128, 128, 0},{128, 128, 128},{255, 0, 255},{255, 255, 0},{255, 128, 0}}};
			  uint32_t colors[] = {
				rgba(255, 255, 255), // White
				rgba(0, 255, 0), // Green
				rgba(0, 0, 255), // Blue
				rgba(255, 0, 0), // Red
				rgba(0, 128, 128), // Teal
				rgba(0, 0, 128), // Navy
				rgba(0, 128, 0), // Green
				rgba(0, 255, 255), // Aqua
				rgba(128, 0, 0), // Dark Green? It's dark red
				rgba(192, 192, 192), // Silver
				rgba(128, 0, 128), // Purple
				rgba(128, 128, 0), // Olive
				rgba(128, 128, 128), // Gray
				rgba(255, 0, 255), // Furishia
				rgba(255, 255, 0), // Yellow
				rgba(255, 128, 0), // Orange
			};
			const size_t color_size = sizeof(colors) / sizeof(colors[0]);
			for (size_t i = 0; i < color_size; ++i)
			{
				printf("color %lu/%lu 0x%08X\n", i+1, color_size, colors[i]);
			}
			return 0;
		}
		#undef _rgba
		#undef rgba
		-->
		<!--
		_start:
		# Start addr 0204118c
		LEA RCX, [0x0436bca4] # Show Foreground Stats bool
		CMP byte ptr [RCX], 0
		JZ exit_func
		MOV R13,RAX
		LEA RDI,[RSP + 0x118]
		MOV ESI,0x3
		XOR EDX,EDX
		CALL 0x021aebe0 # Get window context
		vxorps XMM0,XMM0,XMM0
		vxorps XMM1,XMM1,XMM1
		vxorps XMM2,XMM2,XMM2
		vxorps XMM3,XMM3,XMM3
		# coordinates are 1280,720 mapped
		# x1
		MOV EAX, 0x0
		VCVTSI2SS XMM0,XMM0,EAX
		# y1
		MOV EAX, 0x0
		VCVTSI2SS XMM1,XMM1,EAX
		# x2
		MOV EAX, 0x20
		VCVTSI2SS XMM2,XMM2,EAX
		# y2
		MOV EAX, 0x2d0
		VCVTSI2SS XMM3,XMM3,EAX
		LEA RAX,[RSP + 0x118]
		MOV ESI,0xbe141414
		MOV EDX,0xb
		MOV RDI,RAX
		LEA RAX, [0x03400064]
		CMP dword ptr [RAX],0xf # above 16 index, reset
		JA color_default
		CMP dword ptr [RAX],0x0 # check color index
		JZ color_0
		CMP dword ptr [RAX],0x1
		JZ color_1
		CMP dword ptr [RAX],0x2
		JZ color_2
		CMP dword ptr [RAX],0x3
		JZ color_3
		CMP dword ptr [RAX],0x4
		JZ color_4
		CMP dword ptr [RAX],0x5
		JZ color_5
		CMP dword ptr [RAX],0x6
		JZ color_6
		CMP dword ptr [RAX],0x7
		JZ color_7
		CMP dword ptr [RAX],0x8
		JZ color_8
		CMP dword ptr [RAX],0x9
		JZ color_9
		CMP dword ptr [RAX],0xa
		JZ color_10
		CMP dword ptr [RAX],0xb
		JZ color_11
		CMP dword ptr [RAX],0xc
		JZ color_12
		CMP dword ptr [RAX],0xd
		JZ color_13
		CMP dword ptr [RAX],0xe
		JZ color_14
		CMP dword ptr [RAX],0xf
		JZ color_15
		color_default:
		MOV ESI, 0xFFFFFFFF # first index color
		MOV dword ptr [RAX],1 # next frame, use second index
		JMP draw_bar_only_no_add
		color_0:
		MOV ESI, 0xFFFFFFFF # White
		JMP draw_bar
		color_1:
		MOV ESI, 0xFF00FF00 # Lime (Green)
		JMP draw_bar
		color_2:
		MOV ESI, 0xFFFF0000 # Blue
		JMP draw_bar
		color_3:
		MOV ESI, 0xFF0000FF # Red
		JMP draw_bar
		color_4:
		MOV ESI, 0xFF808000 # Teal
		JMP draw_bar
		color_5:
		MOV ESI, 0xFF800000 # Navy
		JMP draw_bar
		color_6:
		MOV ESI, 0xFF008000 # Green
		JMP draw_bar
		color_7:
		MOV ESI, 0xFFFFFF00 # Aqua
		JMP draw_bar
		color_8:
		MOV ESI, 0xFF000080 # Dark Green? It's dark red
		JMP draw_bar
		color_9:
		MOV ESI, 0xFFC0C0C0 # Silver
		JMP draw_bar
		color_10:
		MOV ESI, 0xFF800080 # Purple
		JMP draw_bar
		color_11:
		MOV ESI, 0xFF008080 # Olive
		JMP draw_bar
		color_12:
		MOV ESI, 0xFF808080 # Gray
		JMP draw_bar
		color_13:
		MOV ESI, 0xFFFF00FF # Fuchsia
		JMP draw_bar
		color_14:
		MOV ESI, 0xFF00FFFF # Yellow
		JMP draw_bar
		color_15:
		MOV ESI, 0xFF0080FF # Orange
		# JMP draw_bar
		draw_bar:
		ADD dword ptr [RAX],1
		draw_bar_only_no_add:
		CALL 0x021af640
		exit_func:
		JMP 0x20413df
		*/
		// Text
		write_string(app_pid, NO_ASLR(0x02eb1970), "Draw Color Boarder");
		// Draw Bar
		write_bytes(app_pid, NO_ASLR(0x0204118c), "488d0d11ab3202");
		write_bytes(app_pid, NO_ASLR(0x02041193), "803900");
		write_bytes(app_pid, NO_ASLR(0x02041196), "0f843f010000");
		write_bytes(app_pid, NO_ASLR(0x0204119c), "4989c5");
		write_bytes(app_pid, NO_ASLR(0x0204119f), "488dbc2418010000");
		write_bytes(app_pid, NO_ASLR(0x020411a7), "be03000000");
		write_bytes(app_pid, NO_ASLR(0x020411ac), "31d2");
		write_bytes(app_pid, NO_ASLR(0x020411ae), "e82dda1600");
		write_bytes(app_pid, NO_ASLR(0x020411b3), "c5f857c0");
		write_bytes(app_pid, NO_ASLR(0x020411b7), "c5f057c9");
		write_bytes(app_pid, NO_ASLR(0x020411bb), "c5e857d2");
		write_bytes(app_pid, NO_ASLR(0x020411bf), "c5e057db");
		write_bytes(app_pid, NO_ASLR(0x020411c3), "b800000000");
		write_bytes(app_pid, NO_ASLR(0x020411c8), "c5fa2ac0");
		write_bytes(app_pid, NO_ASLR(0x020411cc), "b800000000");
		write_bytes(app_pid, NO_ASLR(0x020411d1), "c5f22ac8");
		write_bytes(app_pid, NO_ASLR(0x020411d5), "b820000000");
		write_bytes(app_pid, NO_ASLR(0x020411da), "c5ea2ad0");
		write_bytes(app_pid, NO_ASLR(0x020411de), "b8d0020000");
		write_bytes(app_pid, NO_ASLR(0x020411e3), "c5e22ad8");
		write_bytes(app_pid, NO_ASLR(0x020411e7), "488d842418010000");
		write_bytes(app_pid, NO_ASLR(0x020411ef), "be141414be");
		write_bytes(app_pid, NO_ASLR(0x020411f4), "ba0b000000");
		write_bytes(app_pid, NO_ASLR(0x020411f9), "4889c7");
		write_bytes(app_pid, NO_ASLR(0x020411fc), "488d0561ee3b01");
		write_bytes(app_pid, NO_ASLR(0x02041203), "83380f");
		write_bytes(app_pid, NO_ASLR(0x02041206), "7750");
		write_bytes(app_pid, NO_ASLR(0x02041208), "833800");
		write_bytes(app_pid, NO_ASLR(0x0204120b), "7458");
		write_bytes(app_pid, NO_ASLR(0x0204120d), "833801");
		write_bytes(app_pid, NO_ASLR(0x02041210), "745a");
		write_bytes(app_pid, NO_ASLR(0x02041212), "833802");
		write_bytes(app_pid, NO_ASLR(0x02041215), "745c");
		write_bytes(app_pid, NO_ASLR(0x02041217), "833803");
		write_bytes(app_pid, NO_ASLR(0x0204121a), "745e");
		write_bytes(app_pid, NO_ASLR(0x0204121c), "833804");
		write_bytes(app_pid, NO_ASLR(0x0204121f), "7460");
		write_bytes(app_pid, NO_ASLR(0x02041221), "833805");
		write_bytes(app_pid, NO_ASLR(0x02041224), "7462");
		write_bytes(app_pid, NO_ASLR(0x02041226), "833806");
		write_bytes(app_pid, NO_ASLR(0x02041229), "7464");
		write_bytes(app_pid, NO_ASLR(0x0204122b), "833807");
		write_bytes(app_pid, NO_ASLR(0x0204122e), "7466");
		write_bytes(app_pid, NO_ASLR(0x02041230), "833808");
		write_bytes(app_pid, NO_ASLR(0x02041233), "7468");
		write_bytes(app_pid, NO_ASLR(0x02041235), "833809");
		write_bytes(app_pid, NO_ASLR(0x02041238), "746a");
		write_bytes(app_pid, NO_ASLR(0x0204123a), "83380a");
		write_bytes(app_pid, NO_ASLR(0x0204123d), "746c");
		write_bytes(app_pid, NO_ASLR(0x0204123f), "83380b");
		write_bytes(app_pid, NO_ASLR(0x02041242), "746e");
		write_bytes(app_pid, NO_ASLR(0x02041244), "83380c");
		write_bytes(app_pid, NO_ASLR(0x02041247), "7470");
		write_bytes(app_pid, NO_ASLR(0x02041249), "83380d");
		write_bytes(app_pid, NO_ASLR(0x0204124c), "7472");
		write_bytes(app_pid, NO_ASLR(0x0204124e), "83380e");
		write_bytes(app_pid, NO_ASLR(0x02041251), "7474");
		write_bytes(app_pid, NO_ASLR(0x02041253), "83380f");
		write_bytes(app_pid, NO_ASLR(0x02041256), "7476");
		write_bytes(app_pid, NO_ASLR(0x02041258), "beffffffff");
		write_bytes(app_pid, NO_ASLR(0x0204125d), "c70001000000");
		write_bytes(app_pid, NO_ASLR(0x02041263), "eb71");
		write_bytes(app_pid, NO_ASLR(0x02041265), "beffffffff");
		write_bytes(app_pid, NO_ASLR(0x0204126a), "eb67");
		write_bytes(app_pid, NO_ASLR(0x0204126c), "be00ff00ff");
		write_bytes(app_pid, NO_ASLR(0x02041271), "eb60");
		write_bytes(app_pid, NO_ASLR(0x02041273), "be0000ffff");
		write_bytes(app_pid, NO_ASLR(0x02041278), "eb59");
		write_bytes(app_pid, NO_ASLR(0x0204127a), "beff0000ff");
		write_bytes(app_pid, NO_ASLR(0x0204127f), "eb52");
		write_bytes(app_pid, NO_ASLR(0x02041281), "be008080ff");
		write_bytes(app_pid, NO_ASLR(0x02041286), "eb4b");
		write_bytes(app_pid, NO_ASLR(0x02041288), "be000080ff");
		write_bytes(app_pid, NO_ASLR(0x0204128d), "eb44");
		write_bytes(app_pid, NO_ASLR(0x0204128f), "be008000ff");
		write_bytes(app_pid, NO_ASLR(0x02041294), "eb3d");
		write_bytes(app_pid, NO_ASLR(0x02041296), "be00ffffff");
		write_bytes(app_pid, NO_ASLR(0x0204129b), "eb36");
		write_bytes(app_pid, NO_ASLR(0x0204129d), "be800000ff");
		write_bytes(app_pid, NO_ASLR(0x020412a2), "eb2f");
		write_bytes(app_pid, NO_ASLR(0x020412a4), "bec0c0c0ff");
		write_bytes(app_pid, NO_ASLR(0x020412a9), "eb28");
		write_bytes(app_pid, NO_ASLR(0x020412ab), "be800080ff");
		write_bytes(app_pid, NO_ASLR(0x020412b0), "eb21");
		write_bytes(app_pid, NO_ASLR(0x020412b2), "be808000ff");
		write_bytes(app_pid, NO_ASLR(0x020412b7), "eb1a");
		write_bytes(app_pid, NO_ASLR(0x020412b9), "be808080ff");
		write_bytes(app_pid, NO_ASLR(0x020412be), "eb13");
		write_bytes(app_pid, NO_ASLR(0x020412c0), "beff00ffff");
		write_bytes(app_pid, NO_ASLR(0x020412c5), "eb0c");
		write_bytes(app_pid, NO_ASLR(0x020412c7), "beffff00ff");
		write_bytes(app_pid, NO_ASLR(0x020412cc), "eb05");
		write_bytes(app_pid, NO_ASLR(0x020412ce), "beff8000ff");
		write_bytes(app_pid, NO_ASLR(0x020412d3), "830001");
		write_bytes(app_pid, NO_ASLR(0x020412d6), "e865e31600");
		write_bytes(app_pid, NO_ASLR(0x020412db), "e9ff000000");
		// 120Hz
		if (g_isPatch120Hz)
		{
			// force 1080p mode for max perf
			// has some issues, will fix later
			if (parseXML(t2ps4_109_1080p_in120Hz))
			{
				write_bytes32(app_pid, NO_ASLR(0x00a17631), 0);
			}
			// no delay
			write_bytes(app_pid, NO_ASLR(0x021bae80), "66c705ef1a1b020000");
			write_bytes(app_pid, NO_ASLR(0x0218e5a3), "e9d911ce00");
			write_bytes(app_pid, NO_ASLR(0x02e6f781), "488d3d48ae5e00");
			write_bytes(app_pid, NO_ASLR(0x02e6f788), "8907");
			write_bytes(app_pid, NO_ASLR(0x02e6f78a), "488d059fac5300");
			write_bytes(app_pid, NO_ASLR(0x02e6f791), "488b00");
			write_bytes(app_pid, NO_ASLR(0x02e6f794), "48050df0a70c", isOffsetVideoModeSupported);
			write_bytes(app_pid, NO_ASLR(0x02e6f79a), "488d3d27738101");
			write_bytes(app_pid, NO_ASLR(0x02e6f7a1), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x02e6f7a3), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x02e6f7a8), "31d2");
			write_bytes(app_pid, NO_ASLR(0x02e6f7aa), "31c9");
			write_bytes(app_pid, NO_ASLR(0x02e6f7ac), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x02e6f7af), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x02e6f7b1), "83f801");
			write_bytes(app_pid, NO_ASLR(0x02e6f7b4), "7527");
			write_bytes(app_pid, NO_ASLR(0x02e6f7b6), "488d0573ac5300");
			write_bytes(app_pid, NO_ASLR(0x02e6f7bd), "488b00");
			write_bytes(app_pid, NO_ASLR(0x02e6f7c0), "48050df0ed5e", isOffsetConfigureOutput);
			write_bytes(app_pid, NO_ASLR(0x02e6f7c6), "488d3dfb728101");
			write_bytes(app_pid, NO_ASLR(0x02e6f7cd), "8b3f");
			write_bytes(app_pid, NO_ASLR(0x02e6f7cf), "be0f000000");
			write_bytes(app_pid, NO_ASLR(0x02e6f7d4), "31d2");
			write_bytes(app_pid, NO_ASLR(0x02e6f7d6), "31c9");
			write_bytes(app_pid, NO_ASLR(0x02e6f7d8), "4531c0");
			write_bytes(app_pid, NO_ASLR(0x02e6f7db), "ffd0");
			write_bytes(app_pid, NO_ASLR(0x02e6f7dd), "85c0");
			write_bytes(app_pid, NO_ASLR(0x02e6f7df), "e9c5ed31ff");
			/*
			write_float32(app_pid, NO_ASLR(0x0138f9d5 + 6), (120.f));
			write_float32(app_pid, NO_ASLR(0x0138f9ec + 3), (1./120.));
			write_float32(app_pid, NO_ASLR(0x013c62be + 6), (120.f));
			write_float32(app_pid, NO_ASLR(0x013c62d8 + 3), (1./120.));
			write_float32(app_pid, NO_ASLR(0x013c71d7 + 6), (120.f));
			write_float32(app_pid, NO_ASLR(0x013c71f1 + 3), (1./120.));
			write_float32(app_pid, NO_ASLR(0x0144e8bd + 6), (120.f));
			write_float32(app_pid, NO_ASLR(0x0144e8d7 + 3), (1./120.));
			*/
			printf_notification("Applied 120Hz Patch");
		}
		break;
	}
	}
}

void DoPatchBF4_124(pid_t app_pid, uint64_t text_base)
{
	/*
	LEA RAX,[0x03576f20]
	MOV RAX,qword ptr [RAX]
	ADD RAX,0x3AE0
	MOV EDI, dword ptr [R12 + 0x2c8]
	MOV ESI,0xf
	XOR EDX,EDX
	XOR ECX,ECX
	XOR R8D,R8D
	CALL RAX
	CMP EAX,1
	JNZ exit
	LEA RAX,[0x03576f20]
	MOV RAX,qword ptr [RAX]
	SUB RAX,0x2f0
	# ADD RAX,0x46B0
	MOV EDI, dword ptr [R12 + 0x2c8]
	MOV ESI,0xf
	XOR EDX,EDX
	XOR ECX,ECX
	XOR R8D,R8D
	CALL RAX
	exit:
	JMP 0x01358f6a
	*/
	// 120hz
	write_bytes(app_pid, NO_ASLR(0x01358f65), "e9734b8d00");
	write_bytes(app_pid, NO_ASLR(0x01c2dadd), "488d051c949401");
	write_bytes(app_pid, NO_ASLR(0x01c2dae4), "488b00");
	write_bytes(app_pid, NO_ASLR(0x01c2dae7), "48050df0a70c", isOffsetVideoModeSupported);
	write_bytes(app_pid, NO_ASLR(0x01c2daed), "418bbc24c8020000");
	write_bytes(app_pid, NO_ASLR(0x01c2daf5), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x01c2dafa), "31d2");
	write_bytes(app_pid, NO_ASLR(0x01c2dafc), "31c9");
	write_bytes(app_pid, NO_ASLR(0x01c2dafe), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x01c2db01), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x01c2db03), "83f801");
	write_bytes(app_pid, NO_ASLR(0x01c2db06), "7526");
	write_bytes(app_pid, NO_ASLR(0x01c2db08), "488d05f1939401");
	write_bytes(app_pid, NO_ASLR(0x01c2db0f), "488b00");
	write_bytes(app_pid, NO_ASLR(0x01c2db12), "48050df0ed5e", isOffsetConfigureOutput);
	write_bytes(app_pid, NO_ASLR(0x01c2db18), "418bbc24c8020000");
	write_bytes(app_pid, NO_ASLR(0x01c2db20), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x01c2db25), "31d2");
	write_bytes(app_pid, NO_ASLR(0x01c2db27), "31c9");
	write_bytes(app_pid, NO_ASLR(0x01c2db29), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x01c2db2c), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x01c2db2e), "e937b472ff");
	printf_notification("Applied 120Hz Patch");
}

void DoPatchKillzone_181(pid_t app_pid, uint64_t text_base)
{
	if (g_isPatch120Hz)
	{
		/*
		CALL 0x0156c218
		LEA RAX,[0x01c61ca0]
		MOV RAX,qword ptr [RAX]
		ADD RAX,0x3AE0
		MOV EDI,dword ptr [0x01c69e30]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		CMP EAX,1
		JNZ exit
		LEA RAX,[0x01c61ca0]
		MOV RAX,qword ptr [RAX]
		SUB RAX,0x2f0
		MOV EDI,dword ptr [0x01c69e30]
		MOV ESI,0xf
		XOR EDX,EDX
		XOR ECX,ECX
		XOR R8D,R8D
		CALL RAX
		exit:
		JMP 0x006fc315
		*/
		// 120Hz
		write_bytes(app_pid, NO_ASLR(0x006fc310), "e9be8ff000");
		write_bytes(app_pid, NO_ASLR(0x016052d3), "e8406ff6ff");
		write_bytes(app_pid, NO_ASLR(0x016052d8), "488d05b1c96500");
		write_bytes(app_pid, NO_ASLR(0x016052df), "488b00");
		write_bytes(app_pid, NO_ASLR(0x016052e2), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x016052e8), "8b3d424b6600");
		write_bytes(app_pid, NO_ASLR(0x016052ee), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x016052f3), "31d2");
		write_bytes(app_pid, NO_ASLR(0x016052f5), "31c9");
		write_bytes(app_pid, NO_ASLR(0x016052f7), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x016052fa), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x016052fc), "83f801");
		write_bytes(app_pid, NO_ASLR(0x016052ff), "7524");
		write_bytes(app_pid, NO_ASLR(0x01605301), "488d0588c96500");
		write_bytes(app_pid, NO_ASLR(0x01605308), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0160530b), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x01605311), "8b3d194b6600");
		write_bytes(app_pid, NO_ASLR(0x01605317), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x0160531c), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0160531e), "31c9");
		write_bytes(app_pid, NO_ASLR(0x01605320), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x01605323), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x01605325), "e9eb6f0fff");
		printf_notification("Applied 120Hz Patch");
	}
}

void DoPatchGravityDaze_101(pid_t app_pid, uint64_t text_base)
{
	write_bytes(app_pid, NO_ASLR(0x0040266a), "c70001000000");
	write_bytes(app_pid, NO_ASLR(0x004152d3), "e904255d01");
	write_bytes(app_pid, NO_ASLR(0x019e77dc), "488d055dff7800");
	write_bytes(app_pid, NO_ASLR(0x019e77e3), "488b00");
	write_bytes(app_pid, NO_ASLR(0x019e77e6), "48050df0a70c", isOffsetVideoModeSupported);
	write_bytes(app_pid, NO_ASLR(0x019e77ec), "418b3e");
	write_bytes(app_pid, NO_ASLR(0x019e77ef), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x019e77f4), "31d2");
	write_bytes(app_pid, NO_ASLR(0x019e77f6), "31c9");
	write_bytes(app_pid, NO_ASLR(0x019e77f8), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x019e77fb), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x019e77fd), "83f801");
	write_bytes(app_pid, NO_ASLR(0x019e7800), "7521");
	write_bytes(app_pid, NO_ASLR(0x019e7802), "488d0537ff7800");
	write_bytes(app_pid, NO_ASLR(0x019e7809), "488b00");
	write_bytes(app_pid, NO_ASLR(0x019e780c), "48050df0ed5e", isOffsetConfigureOutput);
	write_bytes(app_pid, NO_ASLR(0x019e7812), "418b3e");
	write_bytes(app_pid, NO_ASLR(0x019e7815), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x019e781a), "31d2");
	write_bytes(app_pid, NO_ASLR(0x019e781c), "31c9");
	write_bytes(app_pid, NO_ASLR(0x019e781e), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x019e7821), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x019e7823), "e9bfdaa2fe");
	printf_notification("Applied 120Hz Patch");
}

void DoPatchMEC_102(pid_t app_pid, uint64_t text_base)
{
	write_bytes(app_pid, NO_ASLR(0x01f746fa), "e9e76d8300");
	write_bytes(app_pid, NO_ASLR(0x027ab4e6), "c4817a1106");
	write_bytes(app_pid, NO_ASLR(0x027ab4eb), "41813e5555553f");
	write_bytes(app_pid, NO_ASLR(0x027ab4f2), "7507");
	write_bytes(app_pid, NO_ASLR(0x027ab4f4), "41c7060000803f");
	write_bytes(app_pid, NO_ASLR(0x027ab4fb), "e9ff917cff");
	write_bytes(app_pid, NO_ASLR(0x0119c6ab), "e9a00dde01");
	write_bytes(app_pid, NO_ASLR(0x02f7d450), "e8eba779ff");
	write_bytes(app_pid, NO_ASLR(0x02f7d455), "4989de");
	write_bytes(app_pid, NO_ASLR(0x02f7d458), "488d0581922900");
	write_bytes(app_pid, NO_ASLR(0x02f7d45f), "488b00");
	write_bytes(app_pid, NO_ASLR(0x02f7d462), "48050df0a70c", isOffsetVideoModeSupported);
	write_bytes(app_pid, NO_ASLR(0x02f7d468), "418bbe400d0000");
	write_bytes(app_pid, NO_ASLR(0x02f7d46f), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x02f7d474), "31d2");
	write_bytes(app_pid, NO_ASLR(0x02f7d476), "31c9");
	write_bytes(app_pid, NO_ASLR(0x02f7d478), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x02f7d47b), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x02f7d47d), "83f801");
	write_bytes(app_pid, NO_ASLR(0x02f7d480), "7525");
	write_bytes(app_pid, NO_ASLR(0x02f7d482), "488d0557922900");
	write_bytes(app_pid, NO_ASLR(0x02f7d489), "488b00");
	write_bytes(app_pid, NO_ASLR(0x02f7d48c), "48050df0ed5e", isOffsetConfigureOutput);
	write_bytes(app_pid, NO_ASLR(0x02f7d492), "418bbe400d0000");
	write_bytes(app_pid, NO_ASLR(0x02f7d499), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x02f7d49e), "31d2");
	write_bytes(app_pid, NO_ASLR(0x02f7d4a0), "31c9");
	write_bytes(app_pid, NO_ASLR(0x02f7d4a2), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x02f7d4a5), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x02f7d4a7), "e904f221fe");
	printf_notification("Applied 120Hz Patch");
}

void DoPatchNier103(pid_t app_pid, uint64_t text_base)
{
	write_bytes(app_pid, NO_ASLR(0x0106bda6), "e991d75100");
	write_bytes(app_pid, NO_ASLR(0x0158953c), "488d057dc85400");
	write_bytes(app_pid, NO_ASLR(0x01589543), "488b00");
	write_bytes(app_pid, NO_ASLR(0x01589546), "48050df0a70c", isOffsetVideoModeSupported);
	write_bytes(app_pid, NO_ASLR(0x0158954c), "418b7f78");
	write_bytes(app_pid, NO_ASLR(0x01589550), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x01589555), "31d2");
	write_bytes(app_pid, NO_ASLR(0x01589557), "31c9");
	write_bytes(app_pid, NO_ASLR(0x01589559), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x0158955c), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x0158955e), "83f801");
	write_bytes(app_pid, NO_ASLR(0x01589561), "7522");
	write_bytes(app_pid, NO_ASLR(0x01589563), "488d0556c85400");
	write_bytes(app_pid, NO_ASLR(0x0158956a), "488b00");
	write_bytes(app_pid, NO_ASLR(0x0158956d), "48050df0ed5e", isOffsetConfigureOutput);
	write_bytes(app_pid, NO_ASLR(0x01589573), "418b7f78");
	write_bytes(app_pid, NO_ASLR(0x01589577), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x0158957c), "31d2");
	write_bytes(app_pid, NO_ASLR(0x0158957e), "31c9");
	write_bytes(app_pid, NO_ASLR(0x01589580), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x01589583), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x01589585), "e92128aeff");
	// timestep fixes
	// write_float32(app_pid, NO_ASLR(0x0148e7a8), 120.0);
	write_float32(app_pid, NO_ASLR(0x0148e7b0), (1. / 120.));
	// 120fps in cinematics
	write_bytes(app_pid, NO_ASLR(0x0106c5ca), "41be01000000");
	write_bytes(app_pid, NO_ASLR(0x0106c5d0), "660f1f840000000000");
	/*
	write_bytes(app_pid, NO_ASLR(0x0106c5c0), "55");
	write_bytes(app_pid, NO_ASLR(0x0106c5c1), "4889e5");
	write_bytes(app_pid, NO_ASLR(0x0106c5c4), "4156");
	write_bytes(app_pid, NO_ASLR(0x0106c5c6), "53");
	write_bytes(app_pid, NO_ASLR(0x0106c5c7), "4889fb");
	write_bytes(app_pid, NO_ASLR(0x0106c5ca), "8b7f78");
	write_bytes(app_pid, NO_ASLR(0x0106c5cd), "be01000000");
	write_bytes(app_pid, NO_ASLR(0x0106c5d2), "4189f6");
	write_bytes(app_pid, NO_ASLR(0x0106c5d5), "418d76ff");
	write_bytes(app_pid, NO_ASLR(0x0106c5d9), "e802104200");
	write_bytes(app_pid, NO_ASLR(0x0106c5de), "4489b314010000");
	write_bytes(app_pid, NO_ASLR(0x0106c5e5), "5b");
	write_bytes(app_pid, NO_ASLR(0x0106c5e6), "415e");
	write_bytes(app_pid, NO_ASLR(0x0106c5e8), "5d");
	write_bytes(app_pid, NO_ASLR(0x0106c5e9), "c3");
	*/
	printf_notification("Applied 120Hz Patch");
}

void DoPatchDoom_112(pid_t app_pid, uint64_t text_base)
{
	write_bytes(app_pid, NO_ASLR(0x013f8c20), "e9f292b802");
	write_bytes(app_pid, NO_ASLR(0x03f81f17), "e874f505ff");
	write_bytes(app_pid, NO_ASLR(0x03f81f1c), "488d057d353800");
	write_bytes(app_pid, NO_ASLR(0x03f81f23), "488b00");
	write_bytes(app_pid, NO_ASLR(0x03f81f26), "48050df0a70c", isOffsetVideoModeSupported);
	write_bytes(app_pid, NO_ASLR(0x03f81f2c), "418b3c24");
	write_bytes(app_pid, NO_ASLR(0x03f81f30), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x03f81f35), "31d2");
	write_bytes(app_pid, NO_ASLR(0x03f81f37), "31c9");
	write_bytes(app_pid, NO_ASLR(0x03f81f39), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x03f81f3c), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x03f81f3e), "83f801");
	write_bytes(app_pid, NO_ASLR(0x03f81f41), "7522");
	write_bytes(app_pid, NO_ASLR(0x03f81f43), "488d0556353800");
	write_bytes(app_pid, NO_ASLR(0x03f81f4a), "488b00");
	write_bytes(app_pid, NO_ASLR(0x03f81f4d), "48050df0ed5e", isOffsetConfigureOutput);
	write_bytes(app_pid, NO_ASLR(0x03f81f53), "418b3c24");
	write_bytes(app_pid, NO_ASLR(0x03f81f57), "be0f000000");
	write_bytes(app_pid, NO_ASLR(0x03f81f5c), "31d2");
	write_bytes(app_pid, NO_ASLR(0x03f81f5e), "31c9");
	write_bytes(app_pid, NO_ASLR(0x03f81f60), "4531c0");
	write_bytes(app_pid, NO_ASLR(0x03f81f63), "ffd0");
	write_bytes(app_pid, NO_ASLR(0x03f81f65), "e9bb6c47fd");
	// write_string(app_pid, NO_ASLR(0x034306d1), "1000"); // offcut for second string "0" but you'd never see the description anyways
	write_bytes(app_pid, NO_ASLR(0x024a3ad4), "488d150effb500");
	printf_notification("Applied 120Hz Patch");
	// no logo videos + legal screen
	write_bytes(app_pid, NO_ASLR(0x024d87dd), "48e9b2010000");
	write_bytes(app_pid, NO_ASLR(0x0246f1c1), "eb07");
	printf_notification("Applied no logo videos + legal screen");
}

void DoPatchBPR_103(pid_t app_pid, uint64_t text_base)
{
	if (parseXML(BPR_103_120Hz))
	{
		write_bytes(app_pid, NO_ASLR(0x007739f0), "b878000000"); // 120 Max FPS (int)
		// Enable 120hz after `SceVideoOutOpen`
		write_bytes(app_pid, NO_ASLR(0x00b5a501), "e9fa8da300");
		write_bytes(app_pid, NO_ASLR(0x01593300), "488d0541960600");
		write_bytes(app_pid, NO_ASLR(0x01593307), "488b00");
		write_bytes(app_pid, NO_ASLR(0x0159330a), "48050df0a70c", isOffsetVideoModeSupported);
		write_bytes(app_pid, NO_ASLR(0x01593310), "418b7f20");
		write_bytes(app_pid, NO_ASLR(0x01593314), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x01593319), "31d2");
		write_bytes(app_pid, NO_ASLR(0x0159331b), "31c9");
		write_bytes(app_pid, NO_ASLR(0x0159331d), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x01593320), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x01593322), "83f801");
		write_bytes(app_pid, NO_ASLR(0x01593325), "7522");
		write_bytes(app_pid, NO_ASLR(0x01593327), "488d051a960600");
		write_bytes(app_pid, NO_ASLR(0x0159332e), "488b00");
		write_bytes(app_pid, NO_ASLR(0x01593331), "48050df0ed5e", isOffsetConfigureOutput);
		write_bytes(app_pid, NO_ASLR(0x01593337), "418b7f20");
		write_bytes(app_pid, NO_ASLR(0x0159333b), "be0f000000");
		write_bytes(app_pid, NO_ASLR(0x01593340), "31d2");
		write_bytes(app_pid, NO_ASLR(0x01593342), "31c9");
		write_bytes(app_pid, NO_ASLR(0x01593344), "4531c0");
		write_bytes(app_pid, NO_ASLR(0x01593347), "ffd0");
		write_bytes(app_pid, NO_ASLR(0x01593349), "e9c6715cff");
		printf_notification("Applied 120Hz Patch");
	}
	// Force 1080p mode
	if (parseXML(BPR_103_1080p))
	{
		write_bytes32(app_pid, NO_ASLR(0x00b54ec4), 1920);
		write_bytes32(app_pid, NO_ASLR(0x00b54ece), 1080);
		printf_notification("Applied 1080p Patch");
	}
	if (parseXML(BPR_103_SkipLogos))
	{
		write_bytes(app_pid, NO_ASLR(0x00745c5d), "c6835826c20001");
	}
	if (parseXML(BPR_103_SkipIntroVideo))
	{
		write_bytes(app_pid, NO_ASLR(0x0086840a), "e912010000");
	}
	write_bytes(app_pid, NO_ASLR(0x00b15c97), "4c89fe");
}
