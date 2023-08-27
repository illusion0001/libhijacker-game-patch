#include "game_patch_memory.hpp"
#include "game_patch.hpp"
#include "print.hpp"

void DoPatch_Bloodborne109(pid_t app_pid, u64 text_base)
{
	// skip logos
	write_bytes32(app_pid, NO_ASLR(0x04d99138), 0);
	write_bytes32(app_pid, NO_ASLR(0x04d99154), 0);
	write_bytes32(app_pid, NO_ASLR(0x04d9916e), 0);
	// 60 FPS by Lance
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
	write_bytes(app_pid, NO_ASLR(0x01bf9b71), "e933010000");
	write_bytes(app_pid, NO_ASLR(0x01bf9b76), "90");
	write_bytes(app_pid, NO_ASLR(0x01bf9b77), "90");
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
	write_bytes(app_pid, NO_ASLR(0x020367c7), "488d351cd2c702");
	write_bytes(app_pid, NO_ASLR(0x020369e8), "488d35b5cfc702");
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
	write_wstring(app_pid, NO_ASLR(0x04cb39ea), L"This 60fps patch was");
	write_wstring(app_pid, NO_ASLR(0x04cb39a4), L"Made with love, by Lance McDonald");
	// Patch DLC Save Requirement
	write_bytes(app_pid, NO_ASLR(0x023b6d20), "b801000000");
	// yoink `assert()`
	write_bytes(app_pid, NO_ASLR(0x02fbf178), "4831c0c3");
	// no motion blur
	write_bytes(app_pid, NO_ASLR(0x026a057b), "eb16");
	// no Chromatic Aberration
	write_bytes(app_pid, NO_ASLR(0x0269faa8), "c783ac000000000000009090");
	// Debug Camera
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
#if 0
	// No Vsync
	// write_bytes32(app_pid, NO_ASLR(0x055289f8), 1280);
	// write_bytes32(app_pid, NO_ASLR(0x055289fc), 720);
	write_bytes(app_pid, NO_ASLR(0x025b3271), "4831f6");
	write_bytes(app_pid, NO_ASLR(0x0243487e), "41c74424186f12833a");
	// FCAT boarder
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
#endif
}

void DoPatch_GravityDaze2_111(pid_t app_pid, u64 text_base)
{
	write_bytes(app_pid, NO_ASLR(0x01802d10), "657030315f636f6d00");
	write_bytes(app_pid, NO_ASLR(0x01803125), "657030305f630000");
	write_bytes(app_pid, NO_ASLR(0x0180312d), "657030315f640000");
	write_bytes(app_pid, NO_ASLR(0x01803135), "657030325f610000");
	write_bytes(app_pid, NO_ASLR(0x0180313d), "736d303800");
	write_bytes(app_pid, NO_ASLR(0x01803142), "736d303900");
	write_bytes(app_pid, NO_ASLR(0x01803147), "657030335f700000");
	write_bytes(app_pid, NO_ASLR(0x0180314f), "657030375f660000");
	write_bytes(app_pid, NO_ASLR(0x01803157), "657030375f680000");
	write_bytes(app_pid, NO_ASLR(0x0180315f), "657030385f620000");
	write_bytes(app_pid, NO_ASLR(0x01803167), "657030385f640000");
	write_bytes(app_pid, NO_ASLR(0x0180316f), "657030395f690000");
	write_bytes(app_pid, NO_ASLR(0x01803177), "657031325f660000");
	write_bytes(app_pid, NO_ASLR(0x0180317f), "657031335f636f00");
	write_bytes(app_pid, NO_ASLR(0x01803187), "6570733135000000");
	write_bytes(app_pid, NO_ASLR(0x0180318f), "657031355f650000");
	write_bytes(app_pid, NO_ASLR(0x01803197), "657031365f610000");
	write_bytes(app_pid, NO_ASLR(0x0180319f), "657032325f630000");
	write_bytes(app_pid, NO_ASLR(0x018031bd), "736d313000");
	write_bytes(app_pid, NO_ASLR(0x018031c2), "736d31325f620000");
	write_bytes(app_pid, NO_ASLR(0x018031ca), "736d313300");
	write_bytes(app_pid, NO_ASLR(0x018031d4), "657032375f636f00");
	write_bytes(app_pid, NO_ASLR(0x018031ad), "646d30365f630000");
	write_bytes(app_pid, NO_ASLR(0x018031b5), "646d30365f640000");
	write_bytes(app_pid, NO_ASLR(0x018038e9), "5468697320706174636820776173206372656174656420627920696c6c7573696f6e0a57697468206c6f766520746f2047726176697479205275736820446576656c6f7065727320616e642046616e732e");
	write_bytes(app_pid, NO_ASLR(0x00846980), "be00000000");
	write_bytes(app_pid, NO_ASLR(0x0084dda4), "bf46100000");
	write_bytes(app_pid, NO_ASLR(0x0084dda9), "eb08");
	write_bytes(app_pid, NO_ASLR(0x0084ddab), "90");
	write_bytes(app_pid, NO_ASLR(0x0085e9cc), "e8cd43fa00");
	write_bytes(app_pid, NO_ASLR(0x01802d90), "00247449");
	write_bytes(app_pid, NO_ASLR(0x01802d94), "6b04010000000000");
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
}

void DoPatch_RDR2_129(pid_t app_pid, u64 text_base)
{
	write_bytes(app_pid, NO_ASLR(0x0578ab57), "be00000000");
}

void DoPatch_SOTC_100(pid_t app_pid, u64 text_base)
{
	write_bytes(app_pid, NO_ASLR(0x0042437f), "0f1f440000");
	write_bytes(app_pid, NO_ASLR(0x01390090), "31c0c3");
	/*
	write_bytes(app_pid, BASE_ASLR(0x00495d5b), "41bd400b0000"); // <!-- MOV R13D,0xa00 -->
	write_bytes(app_pid, BASE_ASLR(0x00495d76), "41bf54060000"); // <!-- MOV R15D,0x5a0 -->
	write_bytes(app_pid, BASE_ASLR(0x0057ee6f), "3d54060000"); // <!-- CMP EAX,0x5a0 -->
	*/
}

void DoPatch_BigCollection_100(pid_t app_pid, u64 text_base, u32 idx)
{
	switch (idx)
	{
	case 1:
	{
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
		break;
	}
	case 2:
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
		break;
	}
	case 3:
	{
		write_bytes(app_pid, NO_ASLR(0x009d86a2), "41c686ba39000000");
		break;
	}
	}
}

void DoPatch_TheOrder1886_102(pid_t app_pid, u64 text_base)
{
	// 60 FPS
	write_bytes(app_pid, NO_ASLR(0x008507c4), "01");
	/*
	// 900p + 16:9
	write_bytes(app_pid, NO_ASLR(0x0084cef5), "75");
	write_bytes32(app_pid, NO_ASLR(0x0084cefd), 1600);
	write_bytes32(app_pid, NO_ASLR(0x0084cf07), 900);
	// 16:9 AR
	write_bytes32(app_pid, NO_ASLR(0x0084ce8c), 0x3fe38e39);
	*/
}

void DoPatch_Big4R_100(pid_t app_pid, u64 text_base, u32 idx)
{
	switch (idx)
	{
	case 1: // `eboot.bin`
	{
		write_bytes(app_pid, BASE_ASLR(0x005b087c), "9090c680453300000148e9"); // eww
		break;
	}
	case 2: // `tllr-boot.bin`
	{
		write_bytes(app_pid, BASE_ASLR(0x005b02fc), "9090c680453300000148e9"); // eww
		break;
	}
	}
}

void DoPatch_DemonSouls(pid_t app_pid, u64 text_base, u32 idx)
{
	switch (idx)
	{
		case 0x100:
		{
			/*
			write_bytes(app_pid, BASE_ASLR(0x00ada6c9), "0f1f44000090");
			write_bytes(app_pid, BASE_ASLR(0x00adabcc), "48c7c00b00000090");
			write_bytes(app_pid, BASE_ASLR(0x00adace5), "b800000000");
			write_bytes(app_pid, BASE_ASLR(0x00adadc2), "c60000b00090");
			//write_bytes(app_pid, BASE_ASLR(0x00adadc5), "cc");
			write_bytes(app_pid, BASE_ASLR(0x00ae48f9), "0f1f440000");
			*/
			write_bytes(app_pid, BASE_ASLR(0x00bcd563 + 0x1820848 + 0x5), "31c0c3"); // patch call to return 0 for fliprate
			write_bytes(app_pid, BASE_ASLR(0x00bcd52b + 0x1820890 + 0x5), "31c0c3"); // enforce vsync by not setting tear margin
			// 120 Hz Patch Tests, these didn't work for some reason.
			// 0x2f0 is offset to the Configure function in 4.03 the prx.
			// yours might be different.
			/*
			write_bytes(app_pid, BASE_ASLR(0x00bcd497), "488d0572691f02");
			write_bytes(app_pid, BASE_ASLR(0x00bcd49e), "488b00");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4a1), "482df0020000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4a7), "8bbb98000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4ad), "48c7c60f000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4b4), "48c7c100000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4bb), "48c7c200000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4c2), "49c7c000000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4c9), "4883c408");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4cd), "5b");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4ce), "5d");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4cf), "ffe0");
			*/
			/*
			write_bytes(app_pid, BASE_ASLR(0x00bcd497), "488d0572691f02");
			write_bytes(app_pid, BASE_ASLR(0x00bcd49e), "488b00");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4a1), "482df0020000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4a7), "8bbb98000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4ad), "48c7c60f000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4b4), "48c7c100000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4bb), "48c7c200000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4c2), "49c7c000000000");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4c9), "4883c408");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4cd), "5b");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4ce), "5d");
			write_bytes(app_pid, BASE_ASLR(0x00bcd4cf), "ffe0");
			*/
			break;
		}
	}
}

void DoPatch_DriveClub_128(pid_t app_pid, u64 text_base)
{
	// 60 FPS
	write_bytes(app_pid, NO_ASLR(0x01184800), "b801000000");
	write_bytes(app_pid, NO_ASLR(0x01184725), "48b001");
	write_bytes(app_pid, NO_ASLR(0x00a6306e), "9090");
	// Unlock All Cars + Bikes
	write_bytes(app_pid, NO_ASLR(0x01187caf), "9090");
	write_bytes(app_pid, NO_ASLR(0x01187ce7), "b801000000");
}

void DoPatch_TheLastGuardian_103(pid_t app_pid, u64 text_base)
{
	// 60 FPS
	write_bytes(app_pid, NO_ASLR(0x0105151a), "be00000000");
	write_bytes64(app_pid, NO_ASLR(0x01aec3e8), 0x08700f0008700f00);
	write_bytes64(app_pid, NO_ASLR(0x01aec3f0), 0x0000000000000000);
}

void DoPatch_CTR_121(pid_t app_pid, u64 text_base, u32 idx)
{
	switch (idx)
	{
	case 1:
	{
		write_bytes(app_pid, NO_ASLR(0x017a9f2b), "be00000000");
		write_bytes(app_pid, NO_ASLR(0x017a9f30), "9090909090909090909090909090");
		break;
	}
	case 2:
	{
		write_bytes(app_pid, NO_ASLR(0x017a9f1b), "be00000000");
		write_bytes(app_pid, NO_ASLR(0x017a9f20), "9090909090909090909090909090");
		break;
	}
	}
}

void DoPatch_t1ps4_111(pid_t app_pid, u64 text_base)
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
	write_bytes(app_pid, NO_ASLR(0x01487140), "31c0c3");
	// Skip Startup Logos
	write_bytes(app_pid, NO_ASLR(0x0040214a), "c687e534000001");
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
}

void DoPatch_t2ps4(pid_t app_pid, u64 text_base, u32 idx)
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
		// 60 FPS
		write_bytes(app_pid, NO_ASLR(0x02bbb5f0), "31c0c3");
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
		break;
	}
	}
}

void DoPatch_ACEZioCollection_102(pid_t app_pid, u64 text_base, u32 idx)
{
	switch (idx)
	{
	case 0: // `eboot.bin`
	{
		write_bytes(app_pid, NO_ASLR(0x00c1b6c6), "be00000000");
		break;
	}
	case 1: // `ScimitarAC2.elf`
	{
		write_bytes(app_pid, NO_ASLR(0x00ad18de), "be00000000");
		break;
	}
	case 2: // `ScimitarACB.elf`
	{
		write_bytes(app_pid, NO_ASLR(0x01574466), "be00000000");
		break;
	}
	case 3: // `ScimitarACR.elf`
	{
		write_bytes(app_pid, NO_ASLR(0x014e1705), "be00000000");
		break;
	}
	}
}
