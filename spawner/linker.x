OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

/* this linker scrip is only for spawner.elf */

PHDRS
{
	/*
	 * PF_X = 0x1
	 * PF_W = 0x2
	 * PF_R = 0x4
	 */

	ph_text PT_LOAD FLAGS (0x1);
	ph_data PT_LOAD FLAGS (0x2 | 0x4);
}

SECTIONS
{
	__payload_base = .;
	.text : ALIGN(0x4000)
	{
		PROVIDE_HIDDEN(__text_start = .);
		KEEP (*(.init))
		KEEP (*(.fini))

		*(.text .text.*)

		PROVIDE_HIDDEN(__text_stop = .);
	} : ph_text = 0x90909090

	.rodata : ALIGN(0x4000)
	{
		*(.rodata .rodata.*)
	} : ph_data

	.data.rel.ro : ALIGN(0x4000)
	{
		*(.data.rel.ro .data.rel.ro.*)
	}

	.rela : ALIGN(0x10)
	{
		PROVIDE_HIDDEN(__rela_start = .);
		*(.rela *.rela.*)
		PROVIDE_HIDDEN(__rela_stop = .);
	}

	.data :
	{
		*(.data .data.*)

		. = ALIGN(0x10);

		__imports_start = .;
		KEEP(*(.imports .imports.*))
		__imports_end = .;

		__patches_start = .;
		KEEP(*(.patches .patches.*))
		QUAD(0); BYTE(0); BYTE(0);
		__patches_end = .;

		__bss_start = .;
		*(.bss .bss.*) *(COMMON)
		__bss_end = .;

		. = . + 4;
		. = ALIGN(4);
	}
}
