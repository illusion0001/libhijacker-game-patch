.intel_syntax noprefix
.section .data

# elf
.global daemon_start
.type   daemon_start, @object
.align  4
daemon_start:
    .incbin "../bin/daemon.elf"
daemon_end:
    .global daemon_size
    .type   daemon_size, @object
    .align  4
daemon_size:
    .int    daemon_end - daemon_start
