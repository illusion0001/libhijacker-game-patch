#include "elfldr.hpp"
#include "kernel/proc.hpp"
#include "kernel/rtld.hpp"
#include "sysmodules.hpp"
#include "util.hpp"
#include <ps5/kernel.h>
#include <sys/elf_common.h>
#include <unistd.h>

extern "C" {
	#include <sys/_stdint.h>
	#include <stdint.h>
	#include <sys/elf64.h>
	#include <sys/types.h>
	#include <ps5/payload_main.h>
	int puts(const char *);
	int usleep(unsigned int useconds);
	extern int _master_sock;
	extern int _victim_sock;
}

namespace {

extern uint8_t LIBLOADER_SHELLCODE[282];
extern uint8_t ALLOCATOR_SHELLCODE[729];
extern uint8_t KERNELRW_SHELLCODE[278];

};

namespace nid {

static inline constexpr Nid sceSysmoduleLoadModuleInternal{"39iV5E1HoCk"};
static inline constexpr Nid sceSysmoduleLoadModuleByNameInternal{"CU8m+Qs+HN4"};
static inline constexpr Nid usleep{"QcteRwbsnV0"};
static inline constexpr Nid errno{"9BcDykPmo1I"};
static inline constexpr Nid mmap{"BPE9s9vQQXo"};
static inline constexpr Nid munmap{"UqDGjXA5yUM"};
static inline constexpr Nid sceKernelJitCreateSharedMemory{"avvJ3J0H0EY"};
static inline constexpr Nid socket{"TU-d9PfIHPM"};
static inline constexpr Nid pipe{"-Jp7F+pXxNg"};
static inline constexpr Nid sceKernelDlsym{"LwG8g3niqwA"};
static inline constexpr Nid setsockopt{"fFxGkxF2bVo"};

}

struct NidKeyValue {
	Nid nid; 		// packed to fit in 12 bytes
	uint32_t index; // index into symtab (which is a 32 bit integer)
	constexpr int_fast32_t operator<=>(const Nid &rhs) const {
		return nid <=> rhs;
	}
}; // total size is 16 bytes to allow a memcpy size of a multiple of 16

class NidMap {
	// this implementation is stupid
	// don't try this one at home

	friend struct SymbolLookupTable;

	NidKeyValue *__restrict nids;
	uint_fast32_t size;
	// there is no intention of ever growing this since the symtab size is known

	int_fast32_t binarySearch(const Nid &key) const {
		int_fast32_t lo = 0;
		int_fast32_t hi = size - 1;

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

	static int_fast32_t toIndex(int_fast32_t i) {
		return -(i + 1);
	}

	NidKeyValue &insert(const Nid &key, uint_fast32_t i) {
		if (size++ == i) [[unlikely]] {
			// append
			NidKeyValue &value = nids[i];
			value.nid = key;
			return value;
		}
		__builtin_memcpy(nids + i + 1, nids + i, sizeof(NidKeyValue) * (size - i));
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
		delete[] nids;
		nids = new NidKeyValue[capacity]();
		return *this;
	}
	~NidMap() { delete[] nids; }
	uint_fast32_t length() const { return size; }

	const NidKeyValue *operator[](const Nid &key) const {
		auto index = binarySearch(key);
		if (index < 0) {
			return nullptr;
		}
		return nids + index;
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
		SymbolLookupTable(SymbolLookupTable&&) = default;
		SymbolLookupTable &operator=(SymbolLookupTable&&) = default;
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
			Nid nid;
			fillNid(nid.str, sym);
			return (*this)[nid];
		}

		operator bool() const {
			return lib != nullptr;
		}

		size_t length() const {
			return nids.length();
		}
};

Elf::Elf(Hijacker *hijacker, uint8_t *data) :
		Elf64_Ehdr(*(Elf64_Ehdr *)data), phdrs((Elf64_Phdr*)(data + e_phoff)),
		strtab(), strtabLength(), symtab(), symtabLength(), relatbl(), relaLength(),
		plt(), pltLength(), hijacker(hijacker), textOffset(), imagebase(),
		data(data), libs(nullptr) {
	// TODO check the elf magic stupid
	//hexdump(data, sizeof(Elf64_Ehdr));
}

Elf::~Elf() {
	// this is to ensure that the destructor for SymbolLookupTable is visible
}

bool loadLibraries(Hijacker &hijacker, const Array<String> &paths, Array<SymbolLookupTable> &libs, const size_t reserved);

bool Elf::parseDynamicTable() {
	const Elf64_Dyn *__restrict dyntbl = nullptr;
	for (size_t i = 0; i < e_phnum; i++) {
		const Elf64_Phdr *__restrict phdr = phdrs + i;
		if (phdr->p_type == PT_DYNAMIC) {
			dyntbl = (Elf64_Dyn*)(data + phdr->p_offset);
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
				relatbl = (Elf64_Rela *)(image + dyn->d_un.d_ptr);
				break;
			case DT_RELASZ:
				relaLength = dyn->d_un.d_val / sizeof(Elf64_Rela);
				break;
			case DT_JMPREL:
				plt = (Elf64_Rela *)(image + dyn->d_un.d_ptr);
				break;
			case DT_PLTRELSZ:
				pltLength = dyn->d_un.d_val / sizeof(Elf64_Rela);
				break;
			case DT_SYMTAB:
				symtab = (Elf64_Sym *) (image + dyn->d_un.d_ptr);
				break;
			case DT_STRTAB:
				strtab = (const char *) (image + dyn->d_un.d_ptr);
				break;
			case DT_STRSZ:
				strtabLength = dyn->d_un.d_val;
				break;
			case DT_HASH:
				symtabLength = ((uint32_t *)(image + dyn->d_un.d_ptr))[1];
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
	int preLoadedHandles[6];
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
			preLoadedHandles[handleCount++] = 0x2001;
			continue;
		}
		if (filename == "libSceLibcInternal.so"_sv || filename == "libc.so"_sv) {
			preLoadedHandles[handleCount++] = 0x2;
			continue;
		}
		if (filename == "libSceSysmodule.so"_sv) {
			preLoadedHandles[handleCount++] = 0x11;
			continue;
		}

		names[i++] = StringView{filename.c_str(), filename.length() - 3};
	}

	// remove unset values
	names.shrink(i);

	libs = {handleCount + names.length()};

	if (names.length() > 0) {
		puts("loading libraries");
		if (!loadLibraries(*hijacker, names, libs, handleCount)) {
			__builtin_printf("failed to load libraries\n");
			return false;
		}
	}

	puts("filling symbol tables");
	for (auto i = 0; i < handleCount; i++) {
		auto ptr = hijacker->getLib(preLoadedHandles[i]);
		if (ptr == nullptr) {
			printf("failed to get lib for 0x%x\n", (unsigned int) preLoadedHandles[i]);
			return false;
		}
		SymbolLookupTable &lib = libs[i] = ptr.release();
		lib.fillTable();
	}

	puts("finished process dynamic table");
	return true;
}

struct LibLoaderArgs {
	struct Result {
		int32_t state;
		int32_t err;
	} result;
	uintptr_t sceSysmoduleLoadModuleInternal;
	uintptr_t sceSysmoduleLoadModuleByNameInternal;
	uintptr_t usleep;
	uintptr_t strtab;
	uintptr_t offsets;
	int numOffsets;

	LibLoaderArgs(Hijacker& hijacker, const String &fulltbl, int nlibs) : result({0, 0}) {
		UniquePtr<SharedLib> libSceSysmodule = hijacker.getLib(0x11);
		sceSysmoduleLoadModuleInternal =
			hijacker.getFunctionAddress(libSceSysmodule.get(), nid::sceSysmoduleLoadModuleInternal);
		sceSysmoduleLoadModuleByNameInternal =
			hijacker.getFunctionAddress(libSceSysmodule.get(), nid::sceSysmoduleLoadModuleByNameInternal);
		usleep = hijacker.getLibKernelFunctionAddress(nid::usleep);
		strtab = hijacker.getDataAllocator().allocate(fulltbl.length());
		offsets = hijacker.getDataAllocator().allocate(sizeof(uintptr_t) * nlibs);
		numOffsets = nlibs;
	}
};

bool loadLibraries(Hijacker &hijacker, const Array<String> &names, Array<SymbolLookupTable> &libs, const size_t reserved) {
	const size_t nlibs = names.length();
	String fulltbl{};
	const size_t positionsSize = sizeof(uintptr_t) * nlibs;
	UniquePtr<intptr_t[]> positions{new intptr_t[nlibs]};
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

	LibLoaderArgs args{hijacker, fulltbl, (int) nlibs};
	auto argbuf = hijacker.getDataAllocator().allocate(sizeof(args));
	hijacker.write(argbuf, &args, sizeof(args));
	hijacker.write(args.offsets, positions.get(), positionsSize);
	hijacker.write(args.strtab, fulltbl.c_str(), fulltbl.length());

	ProcessPointer<LibLoaderArgs::Result> res = hijacker.getPointer<LibLoaderArgs::Result>(argbuf);

	// invoke shellcode to load the libraries
	uintptr_t entry = hijacker.getTextAllocator().allocate(sizeof(LIBLOADER_SHELLCODE));
	hijacker.write(entry, LIBLOADER_SHELLCODE);
	{
		ScopedSuspender suspender{&hijacker};
		auto frame = hijacker.getTrapFrame();
		frame->setRdi(argbuf)
			.setRip(entry)
			.flush();

		hijacker.resume();
		LibLoaderArgs::Result state{0, 0};
		do {
			usleep(10);
			state = *res;
		} while (state.state == 0);

		if (state.state != 1) [[unlikely]] {
			printf("state %d err %d\n", state.state, state.err);
			if (state.err > 0 && (size_t)state.err < nlibs) {
				printf("failed to load lib %s 0x%08llx\n", names[state.err].c_str(), positions[state.err]);
			}
			return false;
		}
		hijacker.read(args.offsets, positions.get(), positionsSize);
	}

	for (size_t i = 0; i < nlibs; i++) {
		intptr_t handle = positions[i];
		UniquePtr<SharedLib> ptr = nullptr;
		if (handle == 0) {
			// sceSysmoduleLoadModuleInternal was used
			ptr = hijacker.getLib(names[i]);
		} else {
			ptr = hijacker.getLib(handle).release();
		}
		if (ptr == nullptr) {
			printf("failed to get lib handle for %s\n", names[i].c_str());
			return false;
		}
		SymbolLookupTable &lib = libs[i + reserved] = ptr.release();
		lib.fillTable();
	}

	return true;
}

static inline bool isLoadable(const Elf64_Phdr *__restrict phdr) {
	return phdr->p_type == PT_LOAD || phdr->p_type == PT_GNU_EH_FRAME;
}

static constexpr uint32_t PROT_READ = 1;
static constexpr uint32_t PROT_WRITE = 2;
static constexpr uint32_t PROT_EXEC = 4;

static inline uint32_t toMmapProt(const Elf64_Phdr *__restrict phdr) {
	uint32_t res = 0;
	if (phdr->p_flags & PF_X) [[unlikely]] {
		res |= PROT_EXEC;
	}
	if (phdr->p_flags & PF_R) [[likely]] {
		res |= PROT_READ;
	}
	if (phdr->p_flags & PF_W) {
		res |= PROT_WRITE;
	}
	return res;
}

struct AllocationInfo {
	uintptr_t paddr;
	uint64_t length;
	uintptr_t result;
	uint32_t protection;
};

struct AllocatorArgs {
	struct Result {
		int32_t state;
		int32_t err;
	} result;
	uintptr_t usleep;
	uintptr_t mmap;
	uintptr_t munmap;
	uintptr_t sceKernelJitCreateSharedMemory;
	uintptr_t errno;
	uintptr_t info;
	int numInfo;

	AllocatorArgs(Hijacker& hijacker, int infoCount) : result({0, 0}) {
		usleep = hijacker.getLibKernelFunctionAddress(nid::usleep);
		mmap = hijacker.getLibKernelFunctionAddress(nid::mmap);
		munmap = hijacker.getLibKernelFunctionAddress(nid::munmap);
		sceKernelJitCreateSharedMemory = hijacker.getLibKernelFunctionAddress(nid::sceKernelJitCreateSharedMemory);
		errno = hijacker.getLibKernelFunctionAddress(nid::errno);
		info = hijacker.getDataAllocator().allocate(sizeof(AllocationInfo) * infoCount);
		numInfo = infoCount;
	}
};

static bool isAlive(int v) {
	for (int pid : dbg::getAllPids()) {
		if (pid == v) {
			return true;
		}
	}
	return false;
}

static uintptr_t runAllocatorShellcode(Hijacker *hijacker, Array<AllocationInfo> &infos, const uintptr_t entry, const size_t loadable) {
	AllocatorArgs args{*hijacker, (int) loadable};
	const auto argbuf = hijacker->getDataAllocator().allocate(sizeof(args));
	hijacker->write(argbuf, &args, sizeof(args));
	hijacker->write(args.info, infos.data(), sizeof(AllocationInfo) * loadable);

	const int pid = hijacker->getPid();
	puts("running allocator shellcode");
	{
		ScopedSuspender suspender{hijacker};

		usleep(10);

		// hold the reference to keep this alive
		auto frame = hijacker->getTrapFrame();
		if (frame == nullptr) [[unlikely]] {
			printf("Process died, null trapframe\n");
			return 0;
		}

		frame->setRdi(argbuf)
			.setRip(entry)
			.flush();

		hijacker->resume();
		printf("waiting for allocator to finish\n");
		do {
			usleep(10);
			if (!isAlive(pid)) {
				printf("spawned process died during allocation\n");
				return 0;
			}
			hijacker->read(argbuf, &args, sizeof(args));
			printf("state: %i\n", args.result.state);
		} while (args.result.state == 0);

		if (args.result.state != 1) [[unlikely]] {
			int32_t state = args.result.state;
			int32_t err = args.result.err;
			if (state == 3) {
				printf("sceKernelJitCreateSharedMemory failed %d\n", err);
			} else {
				printf("Allocator shellcode failed. state: %d, err: %d %s\n", state, err, strerror(err));
			}
			return 0;
		}
	}
	hijacker->read(args.info, infos.data(), sizeof(AllocationInfo) * loadable);
	return infos[0].result;
}

bool Elf::processProgramHeaders() {

	uintptr_t entry = hijacker->getTextAllocator().allocate(sizeof(ALLOCATOR_SHELLCODE));
	hijacker->write(entry, ALLOCATOR_SHELLCODE);

	int text = -1;
	//int ehFrameHdr = -1;
	//int rodata = -1;
	size_t loadable = 0;
	for (auto i = 0; i < e_phnum; i++) {
		const auto *__restrict phdr = phdrs + i;
		if (isLoadable(phdr)) {
			loadable++;
			if (phdr->p_flags & PF_X) [[unlikely]] {
				// we want to ensure .text is first
				text = i;
				textOffset = phdr->p_offset;
			} /*else if (phdr->p_type == PT_GNU_EH_FRAME) [[unlikely]] {
				ehFrameHdr = i;
			} else if (phdr->p_flags == 4) {
				if (rodata == -1) {
					rodata = i;
				}
			}*/
		}
	}
	if (text == -1) {
		puts("no executable section found");
		return false;
	}
	Array<AllocationInfo> infos{loadable};
	const auto *__restrict phdr = phdrs + text;
	infos[0] = {phdr->p_paddr, (phdr->p_memsz + 0x3FFF) & 0xFFFFC000, 0, toMmapProt(phdr)};
	{
		for (size_t i = 0, j = 1; i < e_phnum; i++) {
			const auto *__restrict phdr = phdrs + i;
			if (isLoadable(phdr)) {
				if (phdr->p_flags & PF_X) {
					// skip text
					continue;
				}
				if (phdr->p_align != 0x4000) {
					printf("phdr starting at paddr 0x%llx is not page aligned\n", phdr->p_paddr);
				}
				if ((phdr->p_paddr & 0x3FFF) != 0) [[unlikely]] {
					printf("phdr starting at paddr 0x%llx is not page aligned\n", phdr->p_paddr);
					return false;
				}
				infos[j++] = {phdr->p_paddr, (phdr->p_memsz + 0x3FFF) & 0xFFFFC000, 0, toMmapProt(phdr)};
			}
		}
	}



	imagebase = runAllocatorShellcode(hijacker, infos, entry, loadable);

	if (imagebase == 0) {
		return false;
	}

	// Note: the eh_frame entries in the eboot SharedLib object
	// may need to be set for exceptions to work, idk yet

	// this still doesn't work
	// no punting for now :(
	/*
	if (ehFrameHdr != -1) {
		puts("setting eh_frame metadata");
		const auto *__restrict phdr = phdrs + ehFrameHdr;
		const uint8_t *hdr = data.get() + phdr->p_offset;
		if (hdr[1] != 0x1b) [[unlikely]] {
			printf("unexpected eh_frame_ptr encoding 0x%02x\n", hdr[1]);
			return true;
		}
		const int32_t *data = (int32_t *)(hdr + 4);
		int32_t pos = data[0];
		int32_t numEntries = data[1];
		const uintptr_t ehFrameHdrAddress = imagebase + phdr->p_paddr;
		const uintptr_t ehFrameAddress = ehFrameHdrAddress + 4 + pos;
		constexpr size_t FDE_TABLE_ENTRY_SIZE = 8;
		constexpr size_t FDE_TABLE_OFFSET = 0xc;
		const size_t ehFrameHdrSize = FDE_TABLE_OFFSET + (numEntries * FDE_TABLE_ENTRY_SIZE);
		const size_t ehFrameSize = phdr->p_filesz - ehFrameHdrSize;
		auto eboot = hijacker->getEboot();
		EhFrameInfo info{ehFrameHdrAddress, ehFrameHdrSize, ehFrameAddress, ehFrameSize};
		printf(
			"hdr: 0x%08llx, hdrSize: 0x%08llx, frame: 0x%08llx, frameSize: 0x%08llx\n",
			ehFrameHdrAddress, ehFrameHdrSize, ehFrameAddress, ehFrameSize
		);
		eboot->imagebase(imagebase);
		eboot->setTextAddress(imagebase);
		eboot->setRoDataAddress(imagebase + (phdrs + ehFrameHdr)->p_paddr);
		eboot->setEhFrameInfo(info);
	}
	*/

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

	KernelRWArgs(Hijacker& hijacker) : result({0, 0}) {
		auto &alloc = hijacker.getDataAllocator();
		files = alloc.allocate(sizeof(int[4]));
		usleep = hijacker.getLibKernelFunctionAddress(nid::usleep);
		socket = hijacker.getLibKernelFunctionAddress(nid::socket);
		pipe = hijacker.getLibKernelFunctionAddress(nid::pipe);
		setsockopt = hijacker.getLibKernelFunctionAddress(nid::setsockopt);
		errno = hijacker.getLibKernelFunctionAddress(nid::errno);
	}
};

uintptr_t Elf::setupKernelRW() {
	KernelRWArgs args{*hijacker};
	auto &alloc = hijacker->getDataAllocator();
	const auto argbuf = alloc.allocate(sizeof(args));
	hijacker->write(argbuf, &args, sizeof(args));

	ProcessPointer<int32_t> res = hijacker->getPointer<int32_t>(argbuf);

	const auto code = hijacker->getTextAllocator().allocate(sizeof(KERNELRW_SHELLCODE));
	hijacker->write(code, KERNELRW_SHELLCODE, sizeof(KERNELRW_SHELLCODE));
	{
		ScopedSuspender suspender{hijacker};
		*res = 0;
		hijacker->getTrapFrame()->setRdi(argbuf)
			.setRip(code)
			.flush();

		hijacker->resume();
		while (*res == 0) {
			usleep(10);
		}
	}

	int files[4];
	hijacker->read(args.files, files, sizeof(files));

	if (files[0] == -1 || files[1] == -1 || files[2] == -1 || files[3] == -1) {
		hijacker->read(argbuf, &args, sizeof(args));
		if (args.result.err != 0) {
			printf("failed to obtain master/victim sockets and kernelrw pipes\n%s\n", strerror(args.result.err));
		} else {
			puts("failed to obtain master/victim sockets and kernelrw pipes");
		}
		return 0;
	}

	auto newtbl = hijacker->getProc()->getFdTbl();
	auto sock = newtbl.getFileData(files[0]);
	kwrite<uint32_t>(sock, 0x100);
	auto pcb = kread<uintptr_t>(sock + 0x18);
	auto master_inp6_outputopts = kread<uintptr_t>(pcb + 0x120);
	sock = newtbl.getFileData(files[1]);
	kwrite<uint32_t>(sock, 0x100);
	pcb = kread<uintptr_t>(sock + 0x18);
	auto victim_inp6_outputopts = kread<uintptr_t>(pcb + 0x120);
	kwrite(master_inp6_outputopts + 0x10, victim_inp6_outputopts + 0x10);
	kwrite<uint32_t>(master_inp6_outputopts + 0xc0, 0x13370000);
	const uintptr_t pipeaddr = kread<uintptr_t>(newtbl.getFile(files[2]));

	struct payload_args result = {
		.dlsym = (dlsym_t *) hijacker->getLibKernelFunctionAddress(nid::sceKernelDlsym),
		.rwpipe = ((int *) args.files) + 2,
		.rwpair = (int *) args.files,
		.kpipe_addr = pipeaddr,
		.kdata_base_addr = kernel_base,
		.payloadout = (int *) alloc.allocate(sizeof(int))
	};

	uintptr_t addr = alloc.allocate(sizeof(struct payload_args));
	hijacker->write(addr, &result, sizeof(result));
	return addr;
}

bool Elf::load() {
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
			if (j++ > 10) {
				return false;
			}
		}
	}
	return true;
}

bool Elf::launch() {
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

bool Elf::start(uintptr_t args) {
	ScopedSuspender suspender{hijacker};
	auto frame = hijacker->getTrapFrame();

	const uintptr_t rsp = hijacker->getSavedRsp();

	frame->setRsp(rsp)
		.setRbp(0)
		.setRdi(args)
		.setRip(imagebase + e_entry)
		.flush();

	puts("great success");
	return true;
}

uintptr_t Elf::getSymbolAddress(const Elf64_Rela *__restrict rel) const {
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
	printf("symbol lookup for %s failed\n", strtab + sym->st_name);
	return 0;
}

bool Elf::processRelocations() {
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
				*(uintptr_t*)(image + rel->r_offset) = libsym + rel->r_addend;
				break;
			}
			case R_X86_64_GLOB_DAT: {
				// symbol
				auto libsym = getSymbolAddress(rel);
				if (libsym == 0) [[unlikely]] {
					return false;
				}
				*(uintptr_t*)(image + rel->r_offset) = libsym;
				break;
			}
			case R_X86_64_RELATIVE: {
				// imagebase + addend
				*(uintptr_t*)(image + rel->r_offset) = imagebase + rel->r_addend;
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

bool Elf::processPltRelocations() {
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
		*(uintptr_t*)(image + rel->r_offset) = libsym;
	}

	return true;
}

namespace {

// see shellcode/libloader.cpp for source
uint8_t LIBLOADER_SHELLCODE[]{
    0x48, 0x83, 0xec, 0x28, 0x48, 0x89, 0x7c, 0x24, 0x20, 0xc7, 0x44, 0x24, 0x1c, 0x00, 0x00, 0x00,
    0x00, 0x8b, 0x44, 0x24, 0x1c, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x3b, 0x41, 0x30, 0x0f, 0x8d, 0xda,
    0x00, 0x00, 0x00, 0x48, 0x8b, 0x44, 0x24, 0x20, 0x48, 0x8b, 0x40, 0x28, 0x48, 0x63, 0x4c, 0x24,
    0x1c, 0x48, 0x8b, 0x04, 0xc8, 0x48, 0x89, 0x44, 0x24, 0x10, 0x48, 0xb8, 0x00, 0x00, 0x00, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x48, 0x23, 0x44, 0x24, 0x10, 0x48, 0x83, 0xf8, 0x00, 0x0f, 0x84, 0x1b,
    0x00, 0x00, 0x00, 0x48, 0x8b, 0x44, 0x24, 0x20, 0x48, 0x8b, 0x40, 0x08, 0x48, 0x8b, 0x4c, 0x24,
    0x10, 0x89, 0xcf, 0xff, 0xd0, 0x89, 0x44, 0x24, 0x18, 0xe9, 0x2e, 0x00, 0x00, 0x00, 0x48, 0x8b,
    0x44, 0x24, 0x20, 0x48, 0x8b, 0x40, 0x10, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x8b, 0x79, 0x20,
    0x48, 0x03, 0x7c, 0x24, 0x10, 0x31, 0xc9, 0x41, 0x89, 0xc9, 0x4c, 0x89, 0xce, 0x4c, 0x89, 0xca,
    0x4c, 0x89, 0xc9, 0x4d, 0x89, 0xc8, 0xff, 0xd0, 0x89, 0x44, 0x24, 0x18, 0x83, 0x7c, 0x24, 0x18,
    0x00, 0x0f, 0x8d, 0x2f, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x08, 0x02, 0x00, 0x00, 0x00, 0x8b,
    0x44, 0x24, 0x18, 0x89, 0x44, 0x24, 0x0c, 0x48, 0x8b, 0x44, 0x24, 0x20, 0x48, 0x8b, 0x4c, 0x24,
    0x08, 0x48, 0x89, 0x08, 0x48, 0x8b, 0x44, 0x24, 0x20, 0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0x50,
    0x18, 0xe9, 0xee, 0xff, 0xff, 0xff, 0x48, 0x63, 0x54, 0x24, 0x18, 0x48, 0x8b, 0x44, 0x24, 0x20,
    0x48, 0x8b, 0x40, 0x28, 0x48, 0x63, 0x4c, 0x24, 0x1c, 0x48, 0x89, 0x14, 0xc8, 0x8b, 0x44, 0x24,
    0x1c, 0x83, 0xc0, 0x01, 0x89, 0x44, 0x24, 0x1c, 0xe9, 0x14, 0xff, 0xff, 0xff, 0x48, 0x8b, 0x44,
    0x24, 0x20, 0xc7, 0x00, 0x01, 0x00, 0x00, 0x00, 0x48, 0x8b, 0x44, 0x24, 0x20, 0xbf, 0x40, 0x42,
    0x0f, 0x00, 0xff, 0x50, 0x18, 0xe9, 0xee, 0xff, 0xff, 0xff
};

// see shellcode/allocator.cpp for source
uint8_t ALLOCATOR_SHELLCODE[]{
	0x55, 0x41, 0x57, 0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53, 0x50, 0x44, 0x8b, 0x67, 0x38, 0x48,
	0x89, 0xfb, 0x45, 0x85, 0xe4, 0x7e, 0x14, 0x48, 0x8b, 0x43, 0x30, 0x41, 0x83, 0xfc, 0x11, 0x73,
	0x12, 0x31, 0xc9, 0x45, 0x31, 0xf6, 0xe9, 0x11, 0x01, 0x00, 0x00, 0x45, 0x31, 0xf6, 0xe9, 0x29,
	0x01, 0x00, 0x00, 0x44, 0x89, 0xe1, 0xba, 0x10, 0x00, 0x00, 0x00, 0xc5, 0xf9, 0xef, 0xc0, 0xc5,
	0xf1, 0xef, 0xc9, 0xc5, 0xe9, 0xef, 0xd2, 0xc5, 0xe1, 0xef, 0xdb, 0x83, 0xe1, 0x0f, 0x48, 0x0f,
	0x45, 0xd1, 0x4c, 0x89, 0xe1, 0x48, 0x29, 0xd1, 0x48, 0x8d, 0x90, 0x88, 0x01, 0x00, 0x00, 0x48,
	0x89, 0xce, 0x66, 0x66, 0x66, 0x66, 0x66, 0x2e, 0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xc5, 0xfe, 0x6f, 0xa2, 0x80, 0xfe, 0xff, 0xff, 0xc5, 0xfe, 0x6f, 0xaa, 0xa0, 0xfe, 0xff, 0xff,
	0xc5, 0xfe, 0x6f, 0xb2, 0x00, 0xff, 0xff, 0xff, 0xc5, 0xfe, 0x6f, 0xba, 0x20, 0xff, 0xff, 0xff,
	0xc4, 0xe3, 0x5d, 0x46, 0xa2, 0xc0, 0xfe, 0xff, 0xff, 0x20, 0xc4, 0xe3, 0x55, 0x46, 0xaa, 0xe0,
	0xfe, 0xff, 0xff, 0x20, 0xc5, 0xdd, 0x6c, 0xe5, 0xc4, 0xe3, 0x4d, 0x46, 0xaa, 0x40, 0xff, 0xff,
	0xff, 0x20, 0xc4, 0xe3, 0x45, 0x46, 0xb2, 0x60, 0xff, 0xff, 0xff, 0x20, 0xc5, 0xfe, 0x6f, 0x7a,
	0xa0, 0xc5, 0xdd, 0xd4, 0xc0, 0xc4, 0xe3, 0x45, 0x46, 0x62, 0xe0, 0x20, 0xc5, 0xd5, 0x6c, 0xee,
	0xc5, 0xfe, 0x6f, 0x72, 0x80, 0xc5, 0xd5, 0xd4, 0xc9, 0xc5, 0xfe, 0x6f, 0x6a, 0x20, 0xc4, 0xe3,
	0x4d, 0x46, 0x72, 0xc0, 0x20, 0xc4, 0xe3, 0x55, 0x46, 0x6a, 0x60, 0x20, 0xc5, 0xcd, 0x6c, 0xe4,
	0xc5, 0xfe, 0x6f, 0x32, 0xc5, 0xdd, 0xd4, 0xd2, 0xc4, 0xe3, 0x4d, 0x46, 0x72, 0x40, 0x20, 0x48,
	0x81, 0xc2, 0x00, 0x02, 0x00, 0x00, 0x48, 0x83, 0xc6, 0xf0, 0xc5, 0xcd, 0x6c, 0xe5, 0xc5, 0xdd,
	0xd4, 0xdb, 0x0f, 0x85, 0x58, 0xff, 0xff, 0xff, 0xc5, 0xf5, 0xd4, 0xc0, 0xc5, 0xed, 0xd4, 0xc0,
	0xc5, 0xe5, 0xd4, 0xc0, 0xc4, 0xe3, 0x7d, 0x39, 0xc1, 0x01, 0xc5, 0xf9, 0xd4, 0xc1, 0xc5, 0xf9,
	0x70, 0xc8, 0xee, 0xc5, 0xf9, 0xd4, 0xc1, 0xc4, 0xc1, 0xf9, 0x7e, 0xc6, 0x4c, 0x89, 0xe2, 0x48,
	0x29, 0xca, 0x48, 0xc1, 0xe1, 0x05, 0x48, 0x8d, 0x44, 0x01, 0x08, 0x0f, 0x1f, 0x44, 0x00, 0x00,
	0x4c, 0x03, 0x30, 0x48, 0x83, 0xc0, 0x20, 0x48, 0xff, 0xca, 0x75, 0xf4, 0x4c, 0x8b, 0x6b, 0x10,
	0x31, 0xff, 0x4c, 0x89, 0xf6, 0xba, 0x01, 0x00, 0x00, 0x00, 0xb9, 0x02, 0x10, 0x00, 0x00, 0x41,
	0xb8, 0xff, 0xff, 0xff, 0xff, 0x45, 0x31, 0xc9, 0xc5, 0xf8, 0x77, 0x41, 0xff, 0xd5, 0x48, 0x83,
	0xf8, 0xff, 0x0f, 0x84, 0xf1, 0x00, 0x00, 0x00, 0x48, 0x89, 0xc7, 0x4c, 0x89, 0xf6, 0x49, 0x89,
	0xc7, 0xff, 0x53, 0x18, 0x48, 0x8b, 0x6b, 0x30, 0x48, 0x8d, 0x4c, 0x24, 0x04, 0x31, 0xff, 0xba,
	0x07, 0x00, 0x00, 0x00, 0x4c, 0x8b, 0x75, 0x08, 0xc7, 0x44, 0x24, 0x04, 0xff, 0xff, 0xff, 0xff,
	0x4c, 0x89, 0xf6, 0xff, 0x53, 0x20, 0x85, 0xc0, 0x0f, 0x85, 0xdb, 0x00, 0x00, 0x00, 0x44, 0x8b,
	0x44, 0x24, 0x04, 0x4c, 0x89, 0xff, 0x4c, 0x89, 0xf6, 0xba, 0x04, 0x00, 0x00, 0x00, 0xb9, 0x11,
	0x00, 0x00, 0x00, 0x45, 0x31, 0xc9, 0x41, 0xff, 0xd5, 0x48, 0x89, 0x45, 0x10, 0x48, 0x83, 0xf8,
	0xff, 0x0f, 0x84, 0xd2, 0x00, 0x00, 0x00, 0x41, 0x83, 0xfc, 0x02, 0x7c, 0x4a, 0x49, 0xc1, 0xe4,
	0x05, 0x49, 0x89, 0xc6, 0x45, 0x31, 0xff, 0x49, 0x83, 0xc4, 0xe0, 0x0f, 0x1f, 0x44, 0x00, 0x00,
	0x4a, 0x8b, 0x7c, 0x3d, 0x20, 0x4a, 0x8b, 0x74, 0x3d, 0x28, 0x42, 0x8b, 0x54, 0x3d, 0x38, 0xb9,
	0x12, 0x10, 0x00, 0x00, 0x41, 0xb8, 0xff, 0xff, 0xff, 0xff, 0x45, 0x31, 0xc9, 0x4c, 0x01, 0xf7,
	0x41, 0xff, 0xd5, 0x4a, 0x89, 0x44, 0x3d, 0x30, 0x48, 0x83, 0xf8, 0xff, 0x74, 0x2b, 0x49, 0x83,
	0xc7, 0x20, 0x4d, 0x39, 0xfc, 0x75, 0xc9, 0xc7, 0x03, 0x01, 0x00, 0x00, 0x00, 0x48, 0x8b, 0x5b,
	0x08, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x2e, 0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0xd3, 0xeb, 0xf7, 0xff, 0x53, 0x28, 0x8b, 0x00, 0xc7, 0x03,
	0x05, 0x00, 0x00, 0x00, 0x89, 0x43, 0x04, 0x48, 0x8b, 0x5b, 0x08, 0x0f, 0x1f, 0x44, 0x00, 0x00,
	0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0xd3, 0xeb, 0xf7, 0xff, 0x53, 0x28, 0x8b, 0x00, 0xc7, 0x03,
	0x02, 0x00, 0x00, 0x00, 0x89, 0x43, 0x04, 0x48, 0x8b, 0x5b, 0x08, 0x0f, 0x1f, 0x44, 0x00, 0x00,
	0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0xd3, 0xeb, 0xf7, 0xc7, 0x03, 0x03, 0x00, 0x00, 0x00, 0x89,
	0x43, 0x04, 0x48, 0x8b, 0x5b, 0x08, 0x66, 0x2e, 0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0xd3, 0xeb, 0xf7, 0xff, 0x53, 0x28, 0x8b, 0x00, 0xc7, 0x03,
	0x04, 0x00, 0x00, 0x00, 0x89, 0x43, 0x04, 0x48, 0x8b, 0x5b, 0x08, 0x0f, 0x1f, 0x44, 0x00, 0x00,
	0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0xd3, 0xeb, 0xf7
};

uint8_t KERNELRW_SHELLCODE[]{
	0x41, 0x57, 0x41, 0x56, 0x41, 0x54, 0x53, 0x48, 0x83, 0xec, 0x18, 0x4c, 0x8b, 0x77, 0x18, 0x48,
	0x89, 0xfb, 0xbf, 0x1c, 0x00, 0x00, 0x00, 0xbe, 0x02, 0x00, 0x00, 0x00, 0xba, 0x11, 0x00, 0x00,
	0x00, 0x41, 0xff, 0xd6, 0x4c, 0x8b, 0x7b, 0x08, 0xbf, 0x1c, 0x00, 0x00, 0x00, 0xbe, 0x02, 0x00,
	0x00, 0x00, 0xba, 0x11, 0x00, 0x00, 0x00, 0x41, 0x89, 0x07, 0x41, 0xff, 0xd6, 0x49, 0x8d, 0x7f,
	0x08, 0x41, 0x89, 0x47, 0x04, 0xff, 0x53, 0x20, 0x83, 0xf8, 0xff, 0x0f, 0x84, 0xb8, 0x00, 0x00,
	0x00, 0xc7, 0x04, 0x24, 0x14, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x04, 0x29, 0x00, 0x00, 0x00,
	0xc7, 0x44, 0x24, 0x08, 0x3d, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x0c, 0x00, 0x00, 0x00, 0x00,
	0xc7, 0x44, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x14, 0x00, 0x00, 0x00, 0x00,
	0x49, 0x89, 0xe6, 0xbe, 0x29, 0x00, 0x00, 0x00, 0xba, 0x19, 0x00, 0x00, 0x00, 0x41, 0xb8, 0x18,
	0x00, 0x00, 0x00, 0x4c, 0x8b, 0x63, 0x28, 0x41, 0x8b, 0x3f, 0x4c, 0x89, 0xf1, 0x41, 0xff, 0xd4,
	0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x04, 0x00, 0x00, 0x00, 0x00, 0xc7,
	0x44, 0x24, 0x08, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x0c, 0x00, 0x00, 0x00, 0x00, 0xc7,
	0x44, 0x24, 0x10, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x44, 0x24, 0x14, 0x00, 0x00, 0x00, 0x00, 0xbe,
	0x29, 0x00, 0x00, 0x00, 0xba, 0x2e, 0x00, 0x00, 0x00, 0x4c, 0x89, 0xf1, 0x41, 0xb8, 0x14, 0x00,
	0x00, 0x00, 0x41, 0x8b, 0x7f, 0x04, 0x41, 0xff, 0xd4, 0xc7, 0x03, 0x01, 0x00, 0x00, 0x00, 0x48,
	0x8b, 0x5b, 0x10, 0x66, 0x66, 0x66, 0x66, 0x2e, 0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xbf, 0x40, 0x42, 0x0f, 0x00, 0xff, 0xd3, 0xeb, 0xf7, 0xff, 0x53, 0x30, 0x8b, 0x00, 0x89, 0x43,
	0x04, 0xe9, 0x3b, 0xff, 0xff, 0xff
};

}
