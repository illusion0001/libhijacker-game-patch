
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/_stdint.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/_iovec.h>
#include "util.hpp"

static constexpr auto json = R"({
  "applicationCategoryType": 33554432,
  "localizedParameters": {
    "defaultLanguage": "en-US",
    "en-US": {
      "titleName": "HomebrewDaemon"
    }
  },
  "titleId": "BREW00000"
}
)"_sv;

static constexpr int SYSCALL_OFFSET = 10;
extern "C" const uintptr_t f_get_authinfo;

extern "C" uintptr_t _nmount = 0; // NOLINT(*)

static void app_init() {
	_nmount = f_get_authinfo + SYSCALL_OFFSET;
}

extern "C" int __attribute__((naked, noinline)) nmount(struct iovec *iov, unsigned int length, int flags) {
	asm (
		"mov $378, %rax\n"
		"call *_nmount(%rip)\n"
		"ret\n"
	);
}

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

bool makeHomebrewApp() {
	// REDIS -> NPXS40028
	app_init();
	if (!remount("/dev/ssd0.system_ex", "/system_ex")) {
		perror("makenewapp remount");
		return false;
	}
	if (mkdir("/system_ex/app/BREW00000", MKDIR_FLAGS) == STUPID_C_ERROR) {
		const int err = errno;
		if (err != EEXIST) {
			perror("makenewapp mkdir /system_ex/app/BREW00000");
			return false;
		}
		puts("BREW00000 already exists, assuming proper installation");
		return true;
	}
	if (!copyfile("/system_ex/app/NPXS40028/eboot.bin", "/system_ex/app/BREW00000/eboot.bin")) {
		puts("failed to copy redis eboot.bin");
		return false;
	}
	if (!mkdir("/system_ex/app/BREW00000/sce_sys")) {
		return false;
	}
	FILE *fp = fopen("/system_ex/app/BREW00000/sce_sys/param.json", "w+");
	if (fp == nullptr) {
		puts("open failed");
		puts(strerror(errno));
		return false;
	}
	fwrite(json.c_str(), 1, json.length(), fp);
	fclose(fp);
	return true;
}

// NOLINTEND(cppcoreguidelines-owning-memory)
