

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/_stdint.h>
#include <sys/stat.h>
#include "util.hpp"

static constexpr auto json = R"({
	"applicationCategoryType": 33554432,
	"localizedParameters": {
		"defaultLanguage": "en-US",
		"en-US": {
			"titleName": "HomebrewTest"
		}
	},
	"titleId": "BREW00000"
}
)"_sv;


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

static constexpr int STUPID_C_ERROR = -1;
static constexpr int MKDIR_FLAGS = 0666;

// NOLINTBEGIN(cppcoreguidelines-owning-memory)

void makenewapp() {
	if (!remount("/dev/ssd0.system_ex", "/system_ex")) {
		puts(strerror(errno));
		return;
	}
	if (mkdir("/system_ex/app/BREW00000", MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			puts(strerror(errno));
			return;
		}
	}
	struct stat st{};
	if (stat("/system_ex/app/NPXS40028/eboot.bin", &st) == STUPID_C_ERROR) {
		puts(strerror(errno));
		return;
	}
	UniquePtr<uint8_t[]> buf = new uint8_t[st.st_size];
	FILE *fp = fopen("/system_ex/app/NPXS40028/eboot.bin", "rb");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return;
	}
	fread(buf.get(), 1, st.st_size, fp);
	fclose(fp);
	fp = fopen("/system_ex/app/BREW00000/eboot.bin", "wb+");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return;
	}
	fwrite(buf.get(), 1, st.st_size, fp);
	fclose(fp);
	if (mkdir("/system_ex/app/BREW00000/sce_sys", MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			puts(strerror(errno));
			return;
		}
	}
	fp = fopen("/system_ex/app/BREW00000/sce_sys/param.json", "w+");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return;
	}
	fwrite(json.c_str(), 1, json.length(), fp);
	fclose(fp);
}

// NOLINTEND(cppcoreguidelines-owning-memory)
