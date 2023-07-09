#pragma once

#include <util.hpp>

static constexpr uint32_t hash(const StringView &str) {
	uint32_t hash = 0;
	for (size_t i = 0; i < str.length(); i++) {
		hash = 31 * hash + (str[i] & 0xff);
	}
	return hash;
}

struct HashedStringView : StringView {
	uint32_t hash;

	constexpr HashedStringView() : StringView(), hash() {}
	explicit constexpr HashedStringView(const StringView &name) : StringView(name), hash(::hash(name)) {}

	constexpr int32_t operator<=>(const HashedStringView &rhs) const {
		const int32_t v = hash - rhs.hash;
		return v != 0 ? v : StringView::operator<=>(rhs);
	}
};

struct SysmoduleMapEntry {
	HashedStringView name;
	uint32_t id;

	constexpr SysmoduleMapEntry() = default;
	constexpr SysmoduleMapEntry(const StringView &name, uint32_t id) : name(name), id(id) {}

	constexpr int32_t operator<=>(const SysmoduleMapEntry &rhs) const {
		const int32_t v = name.hash - rhs.name.hash;
		return v != 0 ? v : name <=> rhs.name;
	}
	constexpr int32_t operator<=>(const HashedStringView &rhs) const {
		return name <=> rhs;
	}
};

template <size_t N>
struct SysmoduleMapEntryArray {
	SysmoduleMapEntry array[N];

	constexpr SysmoduleMapEntryArray(const SysmoduleMapEntry (&array)[N]) {
		__builtin_memcpy(this->array, array, sizeof(array));
	}

	constexpr SysmoduleMapEntry &operator[](size_t i) {
		return array[i];
	}

	constexpr const SysmoduleMapEntry &operator[](size_t i) const {
		return array[i];
	}
};

constexpr void swap(SysmoduleMapEntry& lhs, SysmoduleMapEntry& rhs) {
	SysmoduleMapEntry tmp = rhs;
	rhs = lhs;
	lhs = tmp;
}

template <size_t N>
constexpr void doSort(SysmoduleMapEntryArray<N> &array, size_t left, size_t right) {
	if (left < right) {
		size_t m = left;

		for (size_t i = left + 1; i<right; i++) {
			if (array[i]<array[left]) {
				swap(array[++m], array[i]);
			}
		}

		swap(array[left], array[m]);

		doSort(array, left, m);
		doSort(array, m + 1, right);
	}
}

template <size_t N>
constexpr SysmoduleMapEntryArray<N> sort(const SysmoduleMapEntry (&array)[N]) {
	SysmoduleMapEntryArray<N> sorted = array;
	doSort(sorted, 0, N);
	return sorted;
}

template <size_t N>
class SysmoduleHashMap {
	SysmoduleMapEntryArray<N> entries;

	constexpr int_fast32_t getIndex(const HashedStringView &key) const {
		int_fast32_t lo = 0;
		int_fast32_t hi = N - 1;

		while (lo <= hi) {
			const auto m = (lo + hi) >> 1;
			const auto n = entries[m] <=> key;

			if (n == 0) [[unlikely]] {
				return m;
			}

			if (n < 0)
				lo = m + 1;
			else
				hi = m - 1;
		}
		return -(lo + 1);
	}

	public:
		constexpr SysmoduleHashMap(const SysmoduleMapEntry (&array)[N]) : entries{sort(array)} {}
		constexpr uint32_t operator[](const StringView &key) const {
			HashedStringView hashed{key};
			const int_fast32_t index = getIndex(hashed);
			return index >= 0 ? entries[index].id : 0;
		}
};

static constexpr const SysmoduleHashMap SYSMODULES{{
	{"libSceAudioOut"_sv, 0x80000001},
	{"libSceAudioIn"_sv, 0x80000002},
	{"libSceAvcap"_sv, 0x80000003},
	{"libSceSysCore"_sv, 0x80000004},
	{"libSceCdlgUtilServer"_sv, 0x80000007},
	{"libSceKeyboard"_sv, 0x80000008},
	{"libSceNetCtl"_sv, 0x80000009},
	{"libSceHttp"_sv, 0x8000000A},
	{"libSceSsl"_sv, 0x8000000B},
	{"libSceNpCommon"_sv, 0x8000000C},
	{"libSceNpManager"_sv, 0x8000000D},
	{"libSceNpWebApi"_sv, 0x8000000E},
	{"libSceSaveData"_sv, 0x8000000F},
	{"libSceSystemService"_sv, 0x80000010},
	{"libSceUserService"_sv, 0x80000011},
	{"libSceVisionManager"_sv, 0x80000012},
	{"libSceAc3Enc"_sv, 0x80000013},
	{"libSceAppInstUtil"_sv, 0x80000014},
	{"libSceVdecCore"_sv, 0x80000015},
	{"libSceVencCore"_sv, 0x80000016},
	{"libSceHidControl"_sv, 0x80000017},
	{"libSceCommonDialog"_sv, 0x80000018},
	{"libScePerf"_sv, 0x80000019},
	{"libSceCamera"_sv, 0x8000001A},
	{"libSceNet"_sv, 0x8000001C},
	{"libSceIpmi"_sv, 0x8000001D},
	{"libSceVideoOut"_sv, 0x80000022},
	{"libSceBgft"_sv, 0x8000002A},
	{"libScePad"_sv, 0x80000024},
	{"libSceVideoOut"_sv, 0x80000022},
	{"libSceVcodec"_sv, 0x80000091},
	{"libSceDipsw"_sv, 0x80000029},
	{"libSceSysUtil"_sv, 0x80000026},
	{"libSceAvSetting"_sv, 0x80000021},
	{"libSceRtc"_sv, 0x80000020},
	{"libSceRegMgr"_sv, 0x8000001f}
}};

static_assert(SYSMODULES["libSceSystemService"_sv] != 0, "libSceSystemService not found");
static_assert(SYSMODULES["libDummy"_sv] == 0, "libDummy found?");
