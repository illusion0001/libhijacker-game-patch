#pragma once

extern "C" {
	#include <elf.h>
	#include <sys/elf64.h>
	#include <ps5/payload_main.h>
}

#include "hijacker.hpp"
#include <util.hpp>

struct SymbolLookupTable;

constexpr auto i = sizeof(Elf64_Dyn);

class Elf : Elf64_Ehdr {

	const Elf64_Phdr *__restrict phdrs;
	const char *__restrict strtab;
	size_t strtabLength;
	const Elf64_Sym *__restrict symtab;
	size_t symtabLength;
	const Elf64_Rela *__restrict relatbl;
	size_t relaLength;
	const Elf64_Rela *__restrict plt;
	size_t pltLength;
	Hijacker *__restrict hijacker;
	size_t textOffset;
	uintptr_t imagebase;
	uint8_t *data;
	Array<SymbolLookupTable> libs;

	bool processProgramHeaders();
	bool parseDynamicTable();
	bool processRelocations();
	bool processPltRelocations();
	bool load();
	bool start(uintptr_t args);
	uintptr_t setupKernelRW();
	uintptr_t getSymbolAddress(const Elf64_Rela *__restrict rel) const;

	public:
		Elf(Hijacker *hijacker, uint8_t *data);
		~Elf();

		bool launch();
};
