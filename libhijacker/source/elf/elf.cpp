#include <stddef.h>
#define _MMAP_DECLARED
#include "dbg/dbg.hpp"
#include "elfldr.hpp"
#include "kernel/proc.hpp"
#include "kernel/rtld.hpp"
#include "sysmodules.hpp"
#include "util.hpp"
#include <netinet/in.h>
#include <ps5/kernel.h>
#include <sys/elf_common.h>
#include <sys/socket.h>
#include <unistd.h>

extern "C" {
	#include <sys/_stdint.h>
	#include <stdint.h>
	#include <sys/elf64.h>
	#include <ps5/payload_main.h>
	int puts(const char *);
	int usleep(unsigned int useconds);
	uintptr_t mmap(uintptr_t, size_t, int, int, int, off_t);
	int munmap(uintptr_t addr, uint64_t len);
	int sceKernelJitCreateSharedMemory(uintptr_t addr, size_t length, uint64_t flags, int *p_fd);
	int *__error();
	int sceSysmoduleLoadModuleInternal(uint32_t);
	int sceSysmoduleLoadModuleByNameInternal(const char *fname, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
	int sceKernelDlsym(int handle, const char* symbol, void** addrp);
	extern const int _master_sock;
	extern const int _victim_sock;
	extern const int _rw_pipe[2];
	extern const uint64_t _pipe_addr;
}

#ifndef IPV6_2292PKTOPTIONS
#define IPV6_2292PKTOPTIONS 25 // NOLINT(*)
#endif

namespace {

constexpr size_t NUM_PRELOADED_MODULES = 3;
constexpr int LIBKERNEL_HANDLE = 0x2001;
constexpr int LIBC_HANDLE = 2;
//constexpr int LIBSYSMODULE_HANDLE = 0X11;
constexpr size_t _PAGE_SIZE = 0x4000; // PAGE_SIZE is a c macro set to 0x100000
constexpr size_t PAGE_ALIGN_MASK = _PAGE_SIZE - 1;
constexpr size_t STACK_ALIGN = 0x10;

constexpr uint32_t MAP_SHARED   = 0x1;
constexpr uint32_t MAP_PRIVATE   = 0x2;
constexpr uint32_t MAP_FIXED     = 0x10;
constexpr uint32_t MAP_ANONYMOUS = 0x1000;
constexpr uintptr_t MAP_FAILED = ~0ULL;

};

struct NidKeyValue {
	Nid nid; 		// packed to fit in 12 bytes
	uint32_t index; // index into symtab (which is a 32 bit integer)
	constexpr int_fast64_t operator<=>(const Nid &rhs) const {
		return nid <=> rhs;
	}
}; // total size is 16 bytes to allow a memcpy size of a multiple of 16

class NidMap {
	// this implementation is stupid
	// don't try this one at home

	friend struct SymbolLookupTable;

	UniquePtr<NidKeyValue[]> nids;
	uint_fast32_t size;
	// there is no intention of ever growing this since the symtab size is known

	int_fast64_t binarySearch(const Nid &key) const {
		int_fast64_t lo = 0;
		int_fast64_t hi = static_cast<int_fast64_t>(size) - 1;

		while (lo <= hi) {
			const auto m = (lo + hi) >> 1;
			const auto n = nids[m].nid <=> key;

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

	static int_fast64_t toIndex(int_fast64_t i) {
		return -(i + 1);
	}

	NidKeyValue &insert(const Nid &key, uint_fast32_t i) {
		if (size++ == i) [[unlikely]] {
			// append
			NidKeyValue &value = nids[i];
			value.nid = key;
			return value;
		}
		__builtin_memcpy(nids.get() + i + 1, nids.get() + i, sizeof(NidKeyValue) * (size - i));
		NidKeyValue &value = nids[i];
		value.nid = key;
		return value;
	}

	NidKeyValue &insert(const Nid &key) {
		auto index = binarySearch(key);
		if (index < 0) {
			return insert(key, toIndex(index));
		}
		return nids[index];
	}

	NidMap(decltype(nullptr)) : nids(nullptr), size() {}
	NidMap(uint_fast32_t capacity) : nids(new NidKeyValue[capacity]()), size(0) {}
	NidMap &operator=(uint_fast32_t capacity) {
		nids = new NidKeyValue[capacity]();
		return *this;
	}
	uint_fast32_t length() const { return size; }

	const NidKeyValue *operator[](const Nid &key) const {
		auto index = binarySearch(key);
		if (index < 0) {
			return nullptr;
		}
		return nids.get() + index;
	}
};

struct SymbolLookupTable {

	NidMap nids;
	UniquePtr<SharedLib> lib;

	void fillTable() {
		const rtld::ElfSymbolTable &symbols = lib->getMetaData()->getSymbolTable();
		const auto len = symbols.length();
		// remember to skip the first null symbol
		for (size_t i = 1; i < len; i++) {
			const auto sym = symbols[i];
			auto &kv = nids.insert(sym.nid());
			kv.index = i;
		}
	}

	public:
		SymbolLookupTable() : nids(nullptr), lib(nullptr) {}
		SymbolLookupTable(SharedLib *lib) :
			// while not obvious, nchain == length of symtab
			nids(lib->getMetaData()->nSymbols()), lib(lib){}

		SymbolLookupTable &operator=(SharedLib *lib) {
			nids = lib->getMetaData()->nSymbols();
			this->lib = lib;
			return *this;
		}

		const rtld::ElfSymbol operator[](const Nid &nid) {
			auto *value = nids[nid];
			if (value == nullptr) [[unlikely]] {
				return nullptr;
			}
			return lib->getMetaData()->getSymbolTable()[value->index];
		}

		const rtld::ElfSymbol operator[](const char *sym) {
			Nid nid; // NOLINT(cppcoreguidelines-pro-type-member-init) we're initializing it in fillNid
			fillNid(nid, sym);
			return (*this)[nid];
		}

		explicit operator bool() const {
			return lib != nullptr;
		}

		size_t length() const {
			return nids.length();
		}
};

Elf::~Elf() noexcept {
	// tracer detaches on destruction and the loaded elf runs
} // must be defined after SymbolLookupTable

Elf::Elf(Hijacker *hijacker, uint8_t *data) noexcept :
		Elf64_Ehdr(*reinterpret_cast<Elf64_Ehdr *>(data)), tracer(hijacker->getPid()),
		phdrs(reinterpret_cast<Elf64_Phdr*>(data + e_phoff)), strtab(),
		strtabLength(), symtab(), symtabLength(), relatbl(), relaLength(),
		plt(), pltLength(), hijacker(hijacker), textOffset(), imagebase(),
		data(data), libs(nullptr), mappedMemory(nullptr), jitFd(-1) {
	// TODO check the elf magic stupid
	//hexdump(data, sizeof(Elf64_Ehdr));
}

bool loadLibraries(Hijacker &hijacker, const dbg::Tracer &tracer, const Array<String> &paths, Array<SymbolLookupTable> &libs, const size_t reserved) noexcept;

bool Elf::parseDynamicTable() noexcept {
	const Elf64_Dyn *__restrict dyntbl = nullptr;
	for (size_t i = 0; i < e_phnum; i++) {
		const Elf64_Phdr *__restrict phdr = phdrs + i;
		if (phdr->p_type == PT_DYNAMIC) {
			dyntbl = reinterpret_cast<Elf64_Dyn*>(data + phdr->p_offset);
			break;
		}
	}

	if (dyntbl == nullptr) [[unlikely]] {
		puts("dynamic table not found");
		return false;
	}

	List<const Elf64_Dyn *> neededLibs{};

	uint8_t *const image = data + textOffset;

	for (const Elf64_Dyn *dyn = dyntbl; dyn->d_tag != DT_NULL; dyn++) {
		switch (dyn->d_tag) {
			case DT_NEEDED:
				neededLibs.push_front(dyn);
				break;
			case DT_RELA:
				relatbl = reinterpret_cast<Elf64_Rela *>(image + dyn->d_un.d_ptr);
				break;
			case DT_RELASZ:
				relaLength = dyn->d_un.d_val / sizeof(Elf64_Rela);
				break;
			case DT_JMPREL:
				plt = reinterpret_cast<Elf64_Rela *>(image + dyn->d_un.d_ptr);
				break;
			case DT_PLTRELSZ:
				pltLength = dyn->d_un.d_val / sizeof(Elf64_Rela);
				break;
			case DT_SYMTAB:
				symtab = reinterpret_cast<Elf64_Sym *>(image + dyn->d_un.d_ptr);
				break;
			case DT_STRTAB:
				strtab = reinterpret_cast<const char *>(image + dyn->d_un.d_ptr);
				break;
			case DT_STRSZ:
				strtabLength = dyn->d_un.d_val;
				break;
			case DT_HASH:
				symtabLength = reinterpret_cast<uint32_t *>(image + dyn->d_un.d_ptr)[1];
				break;

			// don't care for now
			case DT_INIT_ARRAY:
			case DT_INIT_ARRAYSZ:
			case DT_FINI_ARRAY:
			case DT_FINI_ARRAYSZ:
			case DT_PREINIT_ARRAY:
			case DT_PREINIT_ARRAYSZ:
			case DT_PLTREL:
				// don't care
			case DT_RELACOUNT:
			case DT_GNU_HASH:
			default:
				break;
		}
	}

	if (strtab == nullptr) [[unlikely]] {
		puts("strtab not found");
	}

	if (strtabLength == 0 && strtab != nullptr) [[unlikely]] {
		puts("strtab size not found but strtab exists");
		return false;
	}

	if (symtabLength == 0) [[unlikely]] {
		puts("symtab size not found");
	}

	if (symtab == nullptr) [[unlikely]] {
		puts("symtab not found");
	}

	if (relatbl == nullptr) [[unlikely]] {
		puts("rela table not found");
	}

	if (plt == nullptr) [[unlikely]] {
		puts("plt table not found");
	}

	if (symtab == nullptr || strtab == nullptr) [[unlikely]] {
		// don't need to proceed
		return true;
	}

	Array<String> names{neededLibs.length()};

	int preLoadedHandles[NUM_PRELOADED_MODULES];
	int handleCount = 0;
	size_t i = 0;
	for (const Elf64_Dyn *lib : neededLibs) {
		StringView filename = strtab + lib->d_un.d_val;
		if (!filename.endswith(".so"_sv)) [[unlikely]] {
			__builtin_printf("unexpected library 0x%llx %s\n", (unsigned long long)lib->d_un.d_val, filename.c_str());
			return false;
		}
		// I really do not want to implement a hashmap
		if (filename.startswith("libkernel"_sv)) {
			*(preLoadedHandles + handleCount++) = LIBKERNEL_HANDLE;
			continue;
		}
		if (filename == "libSceLibcInternal.so"_sv || filename == "libc.so"_sv) {
			*(preLoadedHandles + handleCount++) = LIBC_HANDLE;
			continue;
		}
		//if (filename == "libSceSysmodule.so"_sv) {
		//	*(preLoadedHandles + handleCount++) = LIBSYSMODULE_HANDLE;
		//	continue;
		//}

		names[i++] = StringView{filename.c_str(), filename.length() - 3};
	}

	// remove unset values
	names.shrink(i);

	libs = {handleCount + names.length()};

	if (names.length() > 0) {
		puts("loading libraries");
		if (!loadLibraries(*hijacker, tracer, names, libs, handleCount)) {
			__builtin_printf("failed to load libraries\n");
			return false;
		}
	}

	puts("filling symbol tables");
	for (auto i = 0; i < handleCount; i++) {
		auto ptr = hijacker->getLib(preLoadedHandles[i]);
		if (ptr == nullptr) [[unlikely]] {
			printf("failed to get lib for 0x%x\n", (unsigned int) preLoadedHandles[i]);
			return false;
		}
		SymbolLookupTable &lib = libs[i] = ptr.release();
		lib.fillTable();
	}

	puts("finished process dynamic table");
	return true;
}

class TracedMemory {
	const dbg::Tracer *tracer;
	uintptr_t mem;
	size_t length;

	public:
		TracedMemory(const dbg::Tracer *tracer, uintptr_t mem, size_t length) noexcept :
			tracer(tracer), mem(mem), length(length) {}
		TracedMemory(const TracedMemory&) = delete;
		TracedMemory &operator=(const TracedMemory&) = delete;
		TracedMemory(TracedMemory &&rhs) noexcept : tracer(rhs.tracer), mem(rhs.mem), length(rhs.length) {
			rhs.tracer = nullptr;
		}
		TracedMemory &operator=(TracedMemory &&rhs) noexcept {
			if (tracer != nullptr) {
				tracer->munmap(mem, length);
			}
			tracer = rhs.tracer;
			mem = rhs.mem;
			length = rhs.length;
			rhs.tracer = nullptr;
			return *this;
		}
		~TracedMemory() noexcept {
			if (tracer != nullptr) {
				tracer->munmap(mem, length);
				tracer = nullptr;
			}
		}
		operator uintptr_t() const noexcept {
			return mem;
		}
};

static inline size_t pageAlign(size_t length) {
	return (length + PAGE_ALIGN_MASK) & ~PAGE_ALIGN_MASK;
}

static constexpr int PROT_READ = 1;
static constexpr int PROT_WRITE = 2;
static constexpr int PROT_EXEC = 4;
static constexpr int PROT_GPU_READ = 0x10;
static constexpr int PROT_GPU_WRITE = 0x20;

static bool loadLibrariesInplace(Hijacker &hijacker, const Array<String> &names, Array<SymbolLookupTable> &libs, const size_t reserved) noexcept {
	for (const auto &name : names) {
		const auto id = SYSMODULES[name];
		int handle = id != 0 ? sceSysmoduleLoadModuleInternal(id) :
			sceSysmoduleLoadModuleByNameInternal(name.c_str(), 0, 0, 0, 0, 0);
		if (handle == -1) {
			printf("failed to get lib handle for %s\n", name.c_str());
			return false;
		}
	}

	const auto nlibs = names.length();
	for (size_t i = 0; i < nlibs; i++) {
		auto ptr = hijacker.getLib(names[i]);
		if (ptr == nullptr) [[unlikely]] {
			printf("failed to get lib handle for %s\n", names[i].c_str());
			return false;
		}

		SymbolLookupTable &lib = libs[i + reserved] = ptr.release();
		lib.fillTable();
	}
	return true;
}

bool loadLibraries(Hijacker &hijacker, const dbg::Tracer &tracer, const Array<String> &names, Array<SymbolLookupTable> &libs, const size_t reserved) noexcept {
	if (hijacker.getPid() == getpid()) {
		return loadLibrariesInplace(hijacker, names, libs, reserved);
	}

	static constexpr uint32_t INTERNAL_MASK = 0x80000000;

	const size_t nlibs = names.length();
	String fulltbl{};
	UniquePtr<uintptr_t[]> positions{new uintptr_t[nlibs]};
	{
		size_t i = 0;
		size_t tblSize = 0;
		for (const String &path : names) {
			tblSize += path.length() + 1;
		}
		fulltbl.reserve(tblSize);
		for (const String &path : names) {
			const StringView name{path};
			auto id = SYSMODULES[name];
			if (id != 0) {
				positions[i++] = id;
			} else {
				positions[i++] = fulltbl.length();
				fulltbl += path;
				fulltbl += '\0';
			}
		}
	}

	const uintptr_t strtab = hijacker.getDataAllocator().allocate(fulltbl.length() + 1);

	hijacker.write(strtab, fulltbl.c_str(), fulltbl.length() + 1); // include the null terminator

	const auto &lib = hijacker.getLib("libSceSysmodule.sprx"_sv);
	if (lib == nullptr) [[unlikely]] {
		puts("libSceSysmodule.sprx not loaded");
		return false;
	}

	// int sceSysmoduleLoadModuleInternal(uint32_t id);
	const auto sceSysmoduleLoadModuleInternal = hijacker.getFunctionAddress(lib.get(), nid::sceSysmoduleLoadModuleInternal);
	if (sceSysmoduleLoadModuleInternal == 0) [[unlikely]] {
		puts("sceSysmoduleLoadModuleInternal not found");
		return false;
	}

	// int sceSysmoduleLoadModuleByNameInternal(const char *fname, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
	const auto sceSysmoduleLoadModuleByNameInternal = hijacker.getFunctionAddress(lib.get(), nid::sceSysmoduleLoadModuleByNameInternal);
	if (sceSysmoduleLoadModuleByNameInternal == 0) [[unlikely]] {
		puts("sceSysmoduleLoadModuleByNameInternal not found");
		return false;
	}

	for (size_t i = 0; i < nlibs; i++) {
		int handle = -1;
		if (positions[i] & INTERNAL_MASK) {
			handle = tracer.call<int>(sceSysmoduleLoadModuleInternal, static_cast<uint32_t>(positions[i]));
		} else {
			handle = tracer.call<int>(sceSysmoduleLoadModuleByNameInternal, strtab + positions[i], 0, 0, 0, 0, 0);
		}

		if (handle == -1) [[unlikely]] {
			printf("failed to get lib handle for %s\n", names[i].c_str());
			return false;
		}

		auto ptr = hijacker.getLib(names[i]);
		if (ptr == nullptr) [[unlikely]] {
			printf("failed to get lib handle for %s\n", names[i].c_str());
			return false;
		}

		SymbolLookupTable &lib = libs[i + reserved] = ptr.release();
		lib.fillTable();
	}

	puts("finished loading libraries");
	return true;
}

static inline bool isLoadable(const Elf64_Phdr *__restrict phdr) {
	return phdr->p_type == PT_LOAD || phdr->p_type == PT_GNU_EH_FRAME;
}

static int jitshm_create(uintptr_t addr, size_t length, uint64_t flags) noexcept {
	int fd = -1;
	if (sceKernelJitCreateSharedMemory(addr, length, flags, &fd) != 0) {
		perror("sceKernelJitCreateSharedMemory");
		return -1;
	}
	return fd;
}

static inline int toMmapProt(const Elf64_Phdr *__restrict phdr) noexcept {
	int res = 0;
	if (phdr->p_flags & PF_X) [[unlikely]] {
		res |= PROT_EXEC;
	}
	if (phdr->p_flags & PF_R) [[likely]] {
		res |= PROT_READ | PROT_GPU_READ;
	}
	if (phdr->p_flags & PF_W) {
		res |= PROT_WRITE | PROT_GPU_WRITE;
	}
	return res;
}

bool Elf::processProgramHeaders() noexcept {
	size_t textLength = 0;
	size_t totalSize = 0;
	size_t numLoadable = 0;
	const Elf64_Phdr *__restrict text = nullptr;
	for (auto i = 0; i < e_phnum; i++) {
		const auto *__restrict phdr = phdrs + i;
		if (isLoadable(phdr)) {
			numLoadable++;
			totalSize += pageAlign(phdr->p_memsz);
			if (phdr->p_flags & PF_X) [[unlikely]] {
				text = phdr;
				textLength = pageAlign(phdr->p_memsz);
				textOffset = phdr->p_offset;
			}
		}
	}
	if (textOffset == 0) {
		puts("no executable section found");
		return false;
	}

	const bool inplace = hijacker->getPid() == getpid();

	if (inplace) {
		mappedMemory = {numLoadable};
	}

	uintptr_t mem = inplace ?
		mmap(0, totalSize, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0) :
		tracer.mmap(0, totalSize, PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	if (mem == MAP_FAILED) [[unlikely]] {
		tracer.perror("mmap Elf::processProgramHeaders");
		return false;
	}

	if (inplace) {
		munmap(mem, totalSize);
	} else {
		tracer.munmap(mem, totalSize);
	}

	int fd = inplace ? jitshm_create(0, textLength, PROT_READ | PROT_WRITE | PROT_EXEC | PROT_GPU_READ) :
		tracer.jitshm_create(0, textLength, PROT_READ | PROT_WRITE | PROT_EXEC | PROT_GPU_READ);
	if (fd < 0) [[unlikely]] {
		tracer.perror("mmap Elf::processProgramHeaders Tracer::jitshm_create");
		return false;
	}

	if (inplace) {
		jitFd = fd;
	}

	const auto prot = toMmapProt(text);
	imagebase = inplace ? mmap(mem, textLength, prot, MAP_FIXED | MAP_SHARED, fd, 0) :
		tracer.mmap(mem, textLength, prot, MAP_FIXED | MAP_SHARED, fd, 0);
	if (imagebase == MAP_FAILED) [[unlikely]] {
		tracer.perror("mmap Elf::processProgramHeaders Tracer::mmap text");
		return false;
	}

	if (imagebase != mem) {
		puts("mmap Elf::processProgramHeaders did not give the requested address");
		return false;
	}

	size_t memIndex = 0;
	if (inplace) {
		mappedMemory[memIndex++] = {imagebase, textLength};
	}

	for (int i = 0; i < e_phnum; i++) {
		const auto *__restrict phdr = phdrs + i;
		if (phdr->p_flags & PF_X) [[unlikely]] {
			continue;
		}

		if (!isLoadable(phdr)) {
			continue;
		}

		const auto addr = phdr->p_paddr + imagebase;
		const auto sz = pageAlign(phdr->p_memsz);
		const auto prot = toMmapProt(phdr);
		auto result = inplace ? mmap(addr, sz, prot, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0) :
			tracer.mmap(addr, sz, prot, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (result == MAP_FAILED) [[unlikely]] {
			tracer.perror("mmap Elf::processProgramHeaders Tracer::mmap data");
			return false;
		}
		if (result != addr) {
			puts("mmap Elf::processProgramHeaders mmap did not give the requested address");
			return false;
		}
		if (inplace) {
			mappedMemory[memIndex++] = {addr, sz};
		}
	}

	return true;
}

struct KernelRWArgs {
	struct Result {
		int state;
		int err;
	} result;
	uintptr_t files;
	uintptr_t usleep;
	uintptr_t socket;
	uintptr_t pipe;
	uintptr_t setsockopt;
	uintptr_t errno;

	KernelRWArgs(Hijacker& hijacker) :
			result({0, 0}), usleep(hijacker.getLibKernelFunctionAddress(nid::usleep)),
			socket(hijacker.getLibKernelFunctionAddress(nid::socket)), pipe(hijacker.getLibKernelFunctionAddress(nid::pipe)),
			setsockopt(hijacker.getLibKernelFunctionAddress(nid::setsockopt)), errno(hijacker.getLibKernelFunctionAddress(nid::_errno)) {
		auto &alloc = hijacker.getDataAllocator();
		files = alloc.allocate(sizeof(int[4]));
	}
};

static int rwpipe[2]; // NOLINT(*)
static int rwpair[2]; // NOLINT(*)

static struct InternalPayloadArgs {
	struct payload_args args;
	int payloadout;
} gResult; // NOLINT(*)

static uintptr_t setupKernelRWInplace(const Hijacker& hijacker) {
	rwpipe[0] = _rw_pipe[0];
	rwpipe[1] = _rw_pipe[1];
	rwpair[0] = _master_sock;
	rwpair[1] = _victim_sock;
	gResult.args = {
		.dlsym = reinterpret_cast<dlsym_t*>(hijacker.getLibKernelFunctionAddress(nid::sceKernelDlsym)), // NOLINT(*)
		.rwpipe = rwpipe,
		.rwpair = rwpair,
		.kpipe_addr = _pipe_addr,
		.kdata_base_addr = kernel_base,
		.payloadout = &gResult.payloadout
	};
	return reinterpret_cast<uintptr_t>(&gResult);
}

uintptr_t Elf::setupKernelRW() noexcept {
	if (hijacker->getPid() == getpid()) {
		return setupKernelRWInplace(*hijacker);
	}

	int files[4];
	files[0] = tracer.socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	files[1] = tracer.socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

	printf("master socket: %d\n", files[0]);
	printf("victim socket: %d\n", files[1]);

	if (files[0] == -1 || files[1] == -1) [[unlikely]] {
		tracer.perror("Elf::setupKernelRW socket");
		return 0;
	}

	if (tracer.pipe(files + 2) < 0) [[unlikely]] {
		tracer.perror("Elf::setupKernelRW pipe");
		return 0;
	}

	printf("rw pipes: %d, %d\n", files[2], files[3]);

	unsigned int buf[]{20, IPPROTO_IPV6, IPV6_TCLASS, 0, 0, 0}; // NOLINT(*)

	if (tracer.setsockopt(files[0], IPPROTO_IPV6, IPV6_2292PKTOPTIONS, (void*)buf, sizeof(buf)) == -1) {
		tracer.perror("Elf::setupKernelRW setsockopt master");
	}

	__builtin_memset(buf, 0, sizeof(buf));

	if (tracer.setsockopt(files[1], IPPROTO_IPV6, IPV6_PKTINFO, (void*)buf, sizeof(buf) - sizeof(int)) == -1) {
		tracer.perror("Elf::setupKernelRW setsockopt victim");
	}

	if (!createReadWriteSockets(hijacker->getProc(), files)) {
		puts("failed to create kernelrw sockets");
		return 0;
	}

	auto newtbl = hijacker->getProc()->getFdTbl();
	const uintptr_t pipeaddr = kread<uintptr_t>(newtbl.getFile(files[2]));

	auto regs = tracer.getRegisters();
	regs.rsp(regs.rsp() - sizeof(files));
	const uintptr_t newFiles = regs.rsp();
	hijacker->write(newFiles, files, sizeof(files));
	regs.rsp(regs.rsp() - sizeof(int) - sizeof(payload_args));
	const auto rsp = regs.rsp();
	tracer.setRegisters(regs);

	// NOLINTBEGIN(performance-no-int-to-ptr)
	struct payload_args result = {
		.dlsym = reinterpret_cast<dlsym_t *>(hijacker->getLibKernelFunctionAddress(nid::sceKernelDlsym)),
		.rwpipe = reinterpret_cast<int *>(newFiles) + 2,
		.rwpair = reinterpret_cast<int *>(newFiles),
		.kpipe_addr = pipeaddr,
		.kdata_base_addr = kernel_base,
		.payloadout = reinterpret_cast<int*>(rsp + sizeof(struct payload_args))
	};
	// NOLINTEND(performance-no-int-to-ptr)


	hijacker->write(rsp, &result, sizeof(result));
	return rsp;
}

bool Elf::load() noexcept {
	for (size_t i = 0; i < e_phnum; i++) {
		const Elf64_Phdr *__restrict phdr = phdrs + i;

		if (!isLoadable(phdr)) {
			continue;
		}

		const uintptr_t vaddr = phdr->p_paddr + imagebase;

		if (vaddr == 0) {
			continue;
		}

		int j = 0;
		while (!hijacker->write(vaddr, data + phdr->p_offset, phdr->p_filesz)) {
			printf("failed to write section data for phdr with paddr 0x%08llx\n", phdr->p_paddr);
			if (j++ > 10) { // NOLINT(cppcoreguidelines-avoid-magic-numbers)
				// TODO: find out why I did this
				return false;
			}
		}
	}
	return true;
}

bool Elf::launch() noexcept {
	puts("processing program headers");
	if (!processProgramHeaders()) [[unlikely]] {
		return false;
	}
	puts("processing dynamic table");
	if (!parseDynamicTable()) [[unlikely]] {
		return false;
	}
	puts("processing relocations");
	if (!processRelocations()) [[unlikely]] {
		return false;
	}
	puts("processing plt relocations");
	if (!processPltRelocations()) [[unlikely]] {
		return false;
	}

	puts("setting up kernel rw");
	uintptr_t args = setupKernelRW();
	if (args == 0) [[unlikely]] {
		return false;
	}

	puts("loading into memory");
	if (!load()) [[unlikely]] {
		return false;
	}

	puts("starting");

	return start(args);
}

static void correctRsp(dbg::Registers &regs) noexcept {
	constexpr auto mask = ~(STACK_ALIGN - 1);
	regs.rsp((regs.rsp() & mask) - sizeof(mask));
}

bool Elf::start(uintptr_t args) noexcept {
	if (hijacker->getPid() == getpid()) {
		auto fun = reinterpret_cast<int(*)(uintptr_t)>(imagebase + e_entry); // NOLINT(*)
		bool res = fun(args) == 0;
		for (const auto &mem : mappedMemory) {
			munmap(mem.mem, mem.len);
		}
		close(jitFd);
		return res;
	}
	(void) args;
	dbg::Registers regs = tracer.getRegisters();
	correctRsp(regs);
	printf("imagebase: 0x%08llx\n", imagebase);
	regs.rdi(args);
	regs.rip(imagebase + e_entry);
	tracer.setRegisters(regs);
	// it will run on detatch
	puts("great success");
	return true;
}

uintptr_t Elf::getSymbolAddress(const Elf64_Rela *__restrict rel) const noexcept {
	if (symtab == nullptr || strtab == nullptr) [[unlikely]] {
		return true;
	}
	const Elf64_Sym *__restrict sym = symtab + ELF64_R_SYM(rel->r_info);
	if (sym->st_value != 0) {
		// the symbol exists in our elf
		// this can only occur if you're loading a library instead of an executable
		// this was a mistake and I'm an idiot but it may be useful in the future
		return imagebase + sym->st_value;
	}
	for (auto &lib : libs) {
		auto libsym = lib[strtab + sym->st_name];
		if (libsym && libsym.exported()) {
			return libsym.vaddr();
		}
	}
	if (ELF64_ST_BIND(sym->st_info) == STB_WEAK) {
		return -1;
	}
	printf("symbol lookup for %s failed\n", strtab + sym->st_name);
	return 0;
}

bool Elf::processRelocations() noexcept {
	if (relatbl == nullptr) [[unlikely]] {
		return true;
	}
	uint8_t *const image = data + textOffset;
	const size_t length = relaLength;
	for (size_t i = 0; i < length; i++) {
		const Elf64_Rela *__restrict rel = relatbl + i;
		switch (ELF64_R_TYPE(rel->r_info)) {
			case R_X86_64_64: {
				// symbol + addend
				auto libsym = getSymbolAddress(rel);
				if (libsym == 0) [[unlikely]] {
					return false;
				}
				*reinterpret_cast<uintptr_t*>(image + rel->r_offset) = libsym + rel->r_addend;
				break;
			}
			case R_X86_64_GLOB_DAT: {
				// symbol
				auto libsym = getSymbolAddress(rel);
				if (libsym == 0) [[unlikely]] {
					return false;
				}
				if (libsym == static_cast<uintptr_t>(-1)) [[unlikely]] {
					continue;
				}
				*reinterpret_cast<uintptr_t*>(image + rel->r_offset) = libsym;
				break;
			}
			case R_X86_64_RELATIVE: {
				// imagebase + addend
				*reinterpret_cast<uintptr_t*>(image + rel->r_offset) = imagebase + rel->r_addend;
				break;
			}
			case R_X86_64_JMP_SLOT: {
				// edge case where the dynamic relocation sections are merged
				auto libsym = getSymbolAddress(rel);
				if (libsym == 0) {
					const Elf64_Sym *sym = symtab + ELF64_R_SYM(rel->r_info);
					const char *name = strtab + sym->st_name;
					__builtin_printf("failed to find library symbol %s\n", name);
					return false;
				}
				*reinterpret_cast<uintptr_t*>(image + rel->r_offset) = libsym;
				break;
			}
			default:
				const Elf64_Sym *sym = symtab + ELF64_R_SYM(rel->r_info);
				const char *name = strtab + sym->st_name;
				unsigned int type = ELF64_R_TYPE(rel->r_info);
				__builtin_printf("unexpected relocation type %u for symbol %s\n", type, name);
				return false;
			}
		}
	return true;
}

bool Elf::processPltRelocations() noexcept {
	if (plt == nullptr) [[unlikely]] {
		return true;
	}
	uint8_t *const image = data + textOffset;
	const size_t length = pltLength;
	for (size_t i = 0; i < length; i++) {
		const Elf64_Rela *__restrict rel = plt + i;
		if ((ELF64_R_TYPE(rel->r_info)) != R_X86_64_JMP_SLOT) [[unlikely]] {
			const Elf64_Sym *sym = symtab + ELF64_R_SYM(rel->r_info);
			const char *name = strtab + sym->st_name;
			unsigned int type = ELF64_R_TYPE(rel->r_info);
			__builtin_printf("unexpected relocation type %u for symbol %s\n", type, name);
			return false;
		}
		auto libsym = getSymbolAddress(rel);
		if (libsym == 0) {
			const Elf64_Sym *sym = symtab + ELF64_R_SYM(rel->r_info);
			const char *name = strtab + sym->st_name;
			__builtin_printf("failed to find library symbol %s\n", name);
			return false;
		}
		*reinterpret_cast<uintptr_t*>(image + rel->r_offset) = libsym;
	}

	return true;
}
