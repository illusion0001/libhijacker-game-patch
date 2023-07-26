
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/_stdint.h>
#include <sys/stat.h>
#include "util.hpp"

/*
// 33554432  0x02000000 -> Redis (daemon)
// 65792     0x00010100 -> bdj
// 0         0x00000000 -> astro's playroom
// 67108864  0x04000000 -> rnps bundle
// 131328    0x00020100 -> Playstation Now
// 16777216  0x01000000 -> NPXS40087
// 570425344 0x22000000 -> Remote Play
static constexpr auto json = R"({
	"applicationCategoryType": 65792,
	"attribute": 1652558392,
	"attribute2": 0,
	"attribute3": 4,
	"displayLocation": 155,
	"localizedParameters": {
		"defaultLanguage": "en-US",
		"en-US": {
			"titleName": "HomebrewTest"
		}
	},
	"titleId": "BREW00000"
}
)"_sv;
*/


#include <sys/mount.h>
#include <sys/_iovec.h>

struct NonStupidIovec {
	const void *iov_base;
	size_t iov_length;

	constexpr NonStupidIovec(const char *str) : iov_base(str), iov_length(__builtin_strlen(str)+1) {}
	constexpr NonStupidIovec(const char *str, size_t length) : iov_base(str), iov_length(length) {}
};

constexpr NonStupidIovec operator"" _iov(const char *str, unsigned long len) { return {str, len+1}; }


static bool remount(const char *dev, const char *path) {
	NonStupidIovec iov[]{
		"fstype"_iov, "exfatfs"_iov,
		"fspath"_iov, {path},
		"from"_iov, {dev},
		"large"_iov, "yes"_iov,
		"timezone"_iov, "static"_iov,
		"async"_iov, {nullptr, 0},
		"ignoreacl"_iov, {nullptr, 0}
	};
	constexpr size_t iovlen = sizeof(iov) / sizeof(iov[0]);
	return nmount(reinterpret_cast<struct iovec *>(iov), iovlen, MNT_UPDATE) == 0;
}

/*
static constexpr int STUPID_C_ERROR = -1;
static constexpr int MKDIR_FLAGS = 0666;

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

static bool copyfile(const char *from, const char *to) {
	struct stat st{};
	if (stat(from, &st) == STUPID_C_ERROR) {
		puts(strerror(errno));
		return false;
	}
	UniquePtr<uint8_t[]> buf = new uint8_t[st.st_size];
	FILE *fp = fopen(from, "rb");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return false;
	}
	fread(buf.get(), 1, st.st_size, fp);
	fclose(fp);
	fp = fopen(to, "wb+");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return false;
	}
	fwrite(buf.get(), 1, st.st_size, fp);
	fclose(fp);
	return true;
}
*/

/*
static bool mkdir(const char *path) {
	if (::mkdir(path, MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			puts(strerror(errno));
			return false;
		}
	}
	return true;
}
*/

void makenewapp() {
	if (!remount("/dev/ssd0.system_ex", "/system_ex")) {
		puts(strerror(errno));
		return;
	}
	/*
	if (mkdir("/system_ex/app/BREW00000", MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			puts(strerror(errno));
			return;
		}
	}
	if (!copyfile("/system_ex/app/NPXS40093/eboot.bin", "/system_ex/app/BREW00000/eboot.bin")) {
		return;
	}
	if (!mkdir("/system_ex/app/BREW00000/sce_sys")) {
		return;
	}
	FILE *fp = fopen("/system_ex/app/BREW00000/sce_sys/param.json", "w+");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return;
	}
	fwrite(json.c_str(), 1, json.length(), fp);
	fclose(fp);
	if (!mkdir("/system_ex/app/BREW00000/psm")) {
		return;
	}
	if (!mkdir("/system_ex/app/BREW00000/psm/Application")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/app.exe.sprx", "/system_ex/app/BREW00000/psm/Application/app.exe.sprx")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40140/psm/Application/Sce.Vsh.DiscPlayer.dll.sprx", "/system_ex/app/BREW00000/psm/Application/Sce.Vsh.DiscPlayer.dll.sprx")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40140/sce_sys/icon0.png", "/system_ex/app/BREW00000/sce_sys/icon0.png")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40140/sce_sys/pic1.DDS", "/system_ex/app/BREW00000/sce_sys/pic1.DDS")) {
		return;
	}
	*/
	/*if (!mkdir("/system_ex/app/BREW00000/psm/Application/resource")) {
		return;
	}

	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.HmdSetup.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.HmdSetup.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.Message.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.Message.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.NetCtlAp.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.NetCtlAp.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.NpEula.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.NpEula.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.NpSnsFacebook.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.NpSnsFacebook.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.PlayGo.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.PlayGo.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.SaveData.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.SaveData.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.SocialScreen.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.SocialScreen.rco")) {
		return;
	}
	if (!copyfile("/system_ex/app/NPXS40093/psm/Application/resource/Sce.Cdlg.VrService.rco", "/system_ex/app/BREW00000/psm/Application/resource/Sce.Cdlg.VrService.rco")) {
		return;
	}*/
}

// NOLINTEND(cppcoreguidelines-owning-memory)
