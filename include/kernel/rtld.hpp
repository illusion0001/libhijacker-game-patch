#pragma once

#include "kernel.hpp"
#include "nid.hpp"
#include "util.hpp"
#include <ps5/kernel.h>
#include <sys/_stdint.h>

extern "C" {
	#include <elf.h>
}


class RtldMeta;

// namespace to hold these commonly named classes
// to prevent api conflict in the future
namespace rtld {

class ElfSymbolTable;
class ElfSymbol;
class ElfStringTable;

}

namespace {

static RtldMeta *newRtldMeta(uintptr_t imageBase, uintptr_t addr);
static void deleteRtldMeta(RtldMeta *meta);

static rtld::ElfSymbolTable *newSymbolTable(const RtldMeta *meta);
static void deleteSymbolTable(rtld::ElfSymbolTable *tbl);

static rtld::ElfStringTable *newStringTable(const RtldMeta *meta);
static void deleteStringTable(rtld::ElfStringTable *tbl);

}

enum class SectionType : uint32_t {
	UNKNOWN = 0,
	TEXT = 1,
	XOTEXT = 2,
	RODATA = 4,
	RELRO = 8,
	DATA = 16,
	INVALID = (uint32_t)-1
};

class SharedLibSection : public KernelObject<SharedLibSection, 0x18> {

	mutable SectionType _type;

	public:
		SharedLibSection() {}
		SharedLibSection(uintptr_t addr) : KernelObject(addr), _type(SectionType::INVALID) {}
		SharedLibSection(SharedLibSection &&rhs) = default;
		SharedLibSection &operator=(SharedLibSection &&rhs) = default;
		SharedLibSection(const SharedLibSection &rhs) = default;
		SharedLibSection &operator=(const SharedLibSection &rhs) = default;

		SectionType type() const {
			if (_type == SectionType::INVALID) {
				KPointer<SectionType> ptr{get<uintptr_t, 0>() + 8};
				_type = *ptr;
			}
			return _type;
		}

		uintptr_t start() const {
			return get<uintptr_t, 8>();
		}

		uintptr_t end() const {
			return start() + sectionLength();
		}

		size_t sectionLength() const {
			return get<size_t, 16>();
		}
};

struct EhFrameInfo {
	uintptr_t hdr;
	size_t hdrSize;
	uintptr_t frame;
	size_t frameSize;
};

class SharedLib : public KernelObject<SharedLib, 0x200> {

	mutable String path;
	mutable Array<SharedLibSection> sections;
	mutable UniquePtr<RtldMeta> meta;

	uintptr_t getDynlibData() const {
		return get<uintptr_t, 0x148>();
	}

	public:
		const int pid;

		SharedLib(uintptr_t addr, int pid)
			: KernelObject(addr), path(nullptr), sections(nullptr), meta(nullptr), pid(pid) {}

		int handle() const {
			return get<int, 0x28>();
		}

		StringView getPath() const {
			__builtin_printf("SharedLib::getPath 0x%llx\n", get<uintptr_t, 8>());
			if (path.length() == 0) [[unlikely]] {
				path = getString<8>();
			}
			return {path.c_str(), path.length()};
		}

		const Array<SharedLibSection> &getSections() const {
			if (sections == nullptr) [[unlikely]] {
				size_t numSections = get<size_t, 0x48>();
				uintptr_t ptr = get<uintptr_t, 0x40>();
				sections = Array<SharedLibSection>{numSections};
				for (size_t i = 0; i < numSections; i++) {
					sections[i] = SharedLibSection{ptr + (i * SharedLibSection::length)};
				}
			}
			return sections;
		}

		const SharedLibSection *getTextSection() const {
			for (SharedLibSection &section : getSections()) {
				SectionType type = section.type();
				if (type == SectionType::XOTEXT || type == SectionType::TEXT) {
					return &section;
				}
			}
			return nullptr;
		}

		void setTextAddress(uintptr_t addr) {
			const SharedLibSection *section = getTextSection();
			kwrite<uintptr_t>(section->address() + 8, addr);
		}

		void setRoDataAddress(uintptr_t addr) {
			const SharedLibSection *section = getRoDataSection();
			kwrite<uintptr_t>(section->address() + 8, addr);
		}

		const SharedLibSection *getDataSection() const {
			for (SharedLibSection &section : getSections()) {
				if (section.type() == SectionType::DATA) {
					return &section;
				}
			}
			return nullptr;
		}

		const SharedLibSection *getRoDataSection() const {
			for (SharedLibSection &section : getSections()) {
				if (section.type() == SectionType::RODATA) {
					return &section;
				}
			}
			return nullptr;
		}

		uintptr_t imagebase() const {
			return get<uintptr_t, 0x30>();
		}

		void imagebase(uintptr_t base) {
			kwrite<uintptr_t>(address() + 0x30, base);
		}

		RtldMeta *getMetaData() const {
			if (meta == nullptr) [[unlikely]] {
				uintptr_t addr = getDynlibData();
				if (addr) {
					meta = newRtldMeta(imagebase(), addr);
				}
			}
			return meta.get();
		}

		EhFrameInfo getEhFrameInfo() const {
			return get<EhFrameInfo, 0xf8>();
		}

		void setEhFrameInfo(const EhFrameInfo &info) const {
			kwrite<EhFrameInfo>(address() + 0xf8, info);
		}

		uintptr_t getEhFrameHdr() const {
			return get<uintptr_t, 0xf8>();
		}

		void setEhFrameHdr(uintptr_t value) {
			set<0xf8>(value);
		}

		size_t getEhFrameHdrSize() const {
			return get<size_t, 0x100>();
		}

		void setEhFrameHdrSize(uintptr_t value)  {
			set<0x100>(value);
		}

		uintptr_t getEhFrame() const {
			return get<uintptr_t, 0x108>();
		}

		void setEhFrame(uintptr_t value) {
			set<0x108>(value);
		}

		size_t getEhFrameSize() const {
			return get<size_t, 0x110>();
		}

		void setEhFrameSize(uintptr_t value) {
			set<0x110>(value);
		}

};

class SharedLibIterable {

	uintptr_t addr;
	const int pid;

	public:
		SharedLibIterable(decltype(nullptr)) : addr(), pid() {}
		SharedLibIterable(uintptr_t addr, int pid) : addr(addr), pid(pid) {}
		bool operator!=(decltype(nullptr)) const { return addr != 0; }
		UniquePtr<SharedLib> operator*() {
			return new SharedLib{addr, pid};
		}
		SharedLibIterable &operator++() {
			uintptr_t ptr = 0;
			kernel_copyout(addr, &ptr, sizeof(ptr));
			addr = ptr;
			return *this;
		}
};

struct SharedLibIterator {

	const uintptr_t addr;
	const int pid;

	SharedLibIterator(uintptr_t addr, int pid) : addr(addr), pid(pid) {}
	SharedLibIterable begin() const {
		return {addr, pid};
	}
	decltype(nullptr) end() const {
		return nullptr;
	}

};

class SharedObject : KernelObject<SharedObject, 0x188> {

	mutable UniquePtr<SharedLib> eboot;

	public:
		const int pid;

		SharedObject(uintptr_t addr, int pid)
			: KernelObject(addr), eboot(nullptr), pid(pid) {}

		SharedLibIterator getLibs() const {
			return {get<uintptr_t, 0>(), pid};
		}

		SharedLib *getEboot() const {
			if (eboot == nullptr) [[unlikely]] {
				eboot = new SharedLib{get<uintptr_t, 0>(), pid};
			}
			return eboot.get();
		}

		UniquePtr<SharedLib> getLib(int handle) {
			for (auto lib : getLibs()) {
				if (lib->handle() == handle) {
					return lib;
				}
			}
			return nullptr;
		}

		UniquePtr<SharedLib> getLib(const StringView &name) const {
			for (auto lib : getLibs()) {
				if (lib->getPath().endswith(name)) {
					return lib;
				}
			}
			return nullptr;
		}
};

class RtldMeta : public KernelObject<RtldMeta, 0x118> {

	// mutable members for caching

	mutable rtld::ElfSymbolTable *symTable;
	mutable rtld::ElfStringTable *strTable;

	public:
		const uintptr_t imageBase;
		RtldMeta(uintptr_t imageBase, uintptr_t addr) : KernelObject(addr), symTable(), strTable(), imageBase(imageBase) {}
		~RtldMeta() {
			deleteSymbolTable(symTable);
			deleteStringTable(strTable);
		}

		size_t totalSize() const {
			return get<size_t, 0x20>();
		}

		uintptr_t symtab() const {
			return get<uintptr_t, 0x28>();
		}

		size_t symtabSize() const {
			return get<size_t, 0x30>();
		}

		size_t nSymbols() const {
			return get<size_t, 0x30>() / sizeof(Elf64_Sym);
		}

		uintptr_t strtab() const {
			return get<uintptr_t, 0x38>();
		}

		size_t strtabSize() const {
			return get<size_t, 0x40>();
		}

		const rtld::ElfSymbolTable &getSymbolTable() const {
			if (symTable == nullptr) {
				symTable = newSymbolTable(this);
			}
			return *symTable;
		}

		const rtld::ElfStringTable &getStringTable() const {
			if (strTable == nullptr) {
				strTable = newStringTable(this);
			}
			return *strTable;
		}
};

class rtld::ElfStringTable {

	size_t size;
	UniquePtr<char[]> tbl;

	public:
		ElfStringTable(const RtldMeta *meta) : size(meta->strtabSize()), tbl(new char[size]) {
			kernel_copyout(meta->strtab(), tbl.get(), size);
		}
		ElfStringTable(const ElfStringTable &) = delete;
		ElfStringTable &operator=(const ElfStringTable &) = delete;
		ElfStringTable(ElfStringTable &&) = default;
		ElfStringTable &operator=(ElfStringTable &&) = default;

		StringView operator[](size_t offset) const {
			#ifdef DEBUG
			if (offset >= size) [[unlikely]] {
				fatalf("offset %llu is out of bounds for size %llu\n", offset, size);
			}
			#endif
			return tbl.get() + offset;
		}

		Nid getNid(size_t offset) const {
			Nid nid;
			__builtin_memcpy(nid.str, tbl.get() + offset, 11);
			nid.str[11] = '\0';
			return nid;
		}
};

class rtld::ElfSymbol : public Elf64_Sym {
	const RtldMeta *meta;

	public:
		ElfSymbol(decltype(nullptr)) : Elf64_Sym(), meta(nullptr) {}
		ElfSymbol(const RtldMeta *meta, const Elf64_Sym &sym) : Elf64_Sym(sym), meta(meta) {}

		StringView name() const {
			return meta->getStringTable()[st_name];
		}

		bool exported() const {
			// exported if the binding is global or weak and shndx is not 0
			return (st_info & 0x30) && st_shndx != 0;
		}

		uintptr_t vaddr() const {
			return meta->imageBase + st_value;
		}

		Nid nid() const {
			return meta->getStringTable().getNid(st_name);
		}

		explicit operator bool() const { return meta; }

};


class rtld::ElfSymbolTable {

	const RtldMeta *meta;
	UniquePtr<Elf64_Sym[]> symbols;
	const size_t size;

	Elf64_Sym *begin() const {
		return symbols.get();
	}

	Elf64_Sym *end() const {
		return symbols.get() + size;
	}

	public:
		ElfSymbolTable(const RtldMeta *meta) : meta(meta), symbols(new Elf64_Sym[meta->nSymbols()]), size(meta->nSymbols()) {
			kernel_copyout(meta->symtab(), symbols.get(), size * sizeof(Elf64_Sym));
		}

		const ElfSymbol operator[](size_t i) const {
			#ifdef DEBUG
			if (i >= size) [[unlikely]] {
				fatalf("index %llu is out of bounds for length %llu\n", i, size);
			}
			#endif
			return {meta, symbols.get()[i]};
		}

		const Elf64_Sym *getSymbol(size_t i) const {
			return symbols.get() + i;
		}

		const ElfSymbol operator[](const Nid &nid) const {
			for (Elf64_Sym &esym : *this) {
				ElfSymbol sym{meta, esym};
				if (!sym.exported()) {
					continue;
				}
				if (sym.nid() == nid) {
					return sym;
				}
			}
			return nullptr;
		}

		const ElfSymbol operator[](Nid &&nid) const {
			return this->operator[](nid);
		}

		size_t length() const {
			return size;
		}
};

namespace {

static inline RtldMeta *newRtldMeta(uintptr_t imageBase, uintptr_t addr) {
	return new RtldMeta(imageBase, addr);
}

static inline void deleteRtldMeta(RtldMeta *meta) {
	delete meta;
}

static inline rtld::ElfSymbolTable *newSymbolTable(const RtldMeta *meta) {
	return new rtld::ElfSymbolTable(meta);
}

static inline void deleteSymbolTable(rtld::ElfSymbolTable *tbl) {
	delete tbl;
}

static inline rtld::ElfStringTable *newStringTable(const RtldMeta *meta) {
	return new rtld::ElfStringTable(meta);
}

static inline void deleteStringTable(rtld::ElfStringTable *tbl) {
	delete tbl;
}

}
