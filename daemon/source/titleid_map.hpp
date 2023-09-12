#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef void (*GameHandler)(void *args);

struct TitleId {
	static constexpr auto TITLEID_LENGTH = 10;
	char id[TITLEID_LENGTH]{};

	constexpr int_fast64_t operator<=>(const TitleId& rhs) const {
		const auto a = __builtin_bit_cast(Helper, id);
		const auto b = __builtin_bit_cast(Helper, rhs.id);
		return a <=> b;
	}

	private:
		struct __attribute__((packed)) Helper {
			int_fast64_t low;
			uint16_t hi;
			constexpr int_fast64_t operator<=>(const Helper& rhs) const {
				const auto i = low - rhs.low;
				if (i == 0) [[unlikely]] {
					return hi - rhs.hi;
				}
		    	return i;
			}
		};

		static_assert(sizeof(Helper) == TITLEID_LENGTH);
};

constexpr TitleId operator"" _tid(const char *str, unsigned long len) {
	TitleId tmp{};
	// this will cause an error at compile time if the string is too long
	// include the null terminator
	__builtin_memcpy(tmp.id, str, len + 1);
	return tmp;
}

struct TitleIdKeyValue {
	GameHandler handler = nullptr;
	TitleId id{};

	constexpr int_fast64_t operator<=>(const TitleIdKeyValue &rhs) const {
		return id <=> rhs.id;
	}
};

template <size_t N>
struct TitleIdKeyValueArray {
	// aggregate for compile time sorting
	TitleIdKeyValue array[N];

	constexpr TitleIdKeyValueArray(const TitleIdKeyValue (&array)[N]) {
		__builtin_memcpy(this->array, array, sizeof(array));
	}

	constexpr TitleIdKeyValue &operator[](size_t i) {
		return array[i];
	}

	constexpr const TitleIdKeyValue &operator[](size_t i) const {
		return array[i];
	}
};

constexpr void swap(TitleIdKeyValue& lhs, TitleIdKeyValue& rhs) {
	TitleIdKeyValue tmp = rhs;
	rhs = lhs;
	lhs = tmp;
}

template <size_t N>
constexpr void doSort(TitleIdKeyValueArray<N> &array, size_t left, size_t right) {
	if (left < right) {
		size_t m = left;

		for (size_t i = left + 1; i < right; i++) {
			if (array[i] < array[left]) {
				swap(array[++m], array[i]);
			}
		}

		swap(array[left], array[m]);

		doSort(array, left, m);
		doSort(array, m + 1, right);
	}
}

template <size_t N>
constexpr TitleIdKeyValueArray<N> sort(const TitleIdKeyValue (&array)[N]) {
	TitleIdKeyValueArray<N> sorted = array;
	doSort(sorted, 0, N);
	return sorted;
}

template <size_t N>
class TitleIdMap {
	friend struct SymbolLookupTable;

	TitleIdKeyValueArray<N> entries;

	constexpr int_fast64_t binarySearch(const TitleId &key) const {
		int_fast64_t lo = 0;
		int_fast64_t hi = static_cast<int_fast64_t>(N) - 1;

		while (lo <= hi) {
			const auto m = (lo + hi) >> 1;
			const auto n = entries[m].id <=> key;

			if (n == 0) [[unlikely]] {
				return m;
			}

			if (n < 0) {
				lo = m + 1;
			} else {
				hi = m - 1;
			}
		}
		return -(lo + 1);
	}

	constexpr static int_fast64_t toIndex(int_fast64_t i) {
		return -(i + 1);
	}

	public:
		constexpr TitleIdMap(const TitleIdKeyValue (&array)[N]) : entries{sort(array)} {}
		static constexpr auto length() { return N; }

		constexpr const TitleIdKeyValue *operator[](const TitleId &key) const {
			auto index = binarySearch(key);
			if (index < 0) {
				return nullptr;
			}
			return entries.array + index;
		}
};
