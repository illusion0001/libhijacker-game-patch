#pragma once

#include "kernel.hpp"

class TrapFrame : public KernelObject<TrapFrame, 0x110> {

	public:
		TrapFrame(const TrapFrame &rhs) : KernelObject(rhs) {}
		TrapFrame(uintptr_t addr) : KernelObject(addr) {}

		TrapFrame &setFrame(const TrapFrame *frame) {
			memcpy(buf, frame->buf, sizeof(buf));
			return *this;
		}

		uint64_t getRax() {
			return get<uint64_t, 0x30>();
		}

		TrapFrame &setRax(uint64_t rax) {
			set<0x30>(rax);
			return *this;
		}

		uint64_t getRbx() {
			return get<uint64_t, 0x38>();
		}

		TrapFrame &setRbx(uint64_t rbx) {
			set<0x38>(rbx);
			return *this;
		}

		uint64_t getRcx() {
			return get<uint64_t, 0x18>();
		}


		TrapFrame &setRcx(uint64_t rcx) {
			set<0x18>(rcx);
			return *this;
		}

		uint64_t getRdx() {
			return get<uint64_t, 0x10>();
		}

		TrapFrame &setRdx(uint64_t rdx) {
			set<0x10>(rdx);
			return *this;
		}

		uint64_t getRsi() {
			return get<uint64_t, 0x8>();
		}

		TrapFrame &setRsi(uint64_t rsi) {
			set<0x8>(rsi);
			return *this;
		}

		uint64_t getRdi() {
			return get<uint64_t, 0>();
		}

		TrapFrame &setRdi(uint64_t rdi) {
			set<0>(rdi);
			return *this;
		}

		uint64_t getR8() {
			return get<uint64_t, 0x20>();
		}

		TrapFrame &setR8(uint64_t r8) {
			set<0x20>(r8);
			return *this;
		}

		uint64_t getR9() {
			return get<uint64_t, 0x28>();
		}

		TrapFrame &setR9(uint64_t r9) {
			set<0x28>(r9);
			return *this;
		}

		uint64_t getR10() {
			return get<uint64_t, 0x48>();
		}

		TrapFrame &setR10(uint64_t r10) {
			set<0x48>(r10);
			return *this;
		}

		uint64_t getR11() {
			return get<uint64_t, 0x50>();
		}

		TrapFrame &setR11(uint64_t r11) {
			set<0x50>(r11);
			return *this;
		}

		uint64_t getR12() {
			return get<uint64_t, 0x58>();
		}
		TrapFrame &setR12(uint64_t r12) {
			set<0x58>(r12);
			return *this;
		}

		uint64_t getR13() {
			return get<uint64_t, 0x60>();
		}

		TrapFrame &setR13(uint64_t r13) {
			set<0x60>(r13);
			return *this;
		}

		uint64_t getR14() {
			return get<uint64_t, 0x68>();
		}

		TrapFrame &setR14(uint64_t r14) {
			set<0x68>(r14);
			return *this;
		}

		uint64_t getR15() {
			return get<uint64_t, 0x70>();
		}

		TrapFrame &setR15(uint64_t r15) {
			set<0x70>(r15);
			return *this;
		}

		uint64_t getRbp() {
			return get<uint64_t, 0x40>();
		}

		TrapFrame &setRbp(uint64_t rbp) {
			set<0x40>(rbp);
			return *this;
		}

		uint64_t getRsp() {
			return get<uint64_t, 0x100>();
		}

		TrapFrame &setRsp(uint64_t rsp) {
			set<0x100>(rsp);
			return *this;
		}

		uint64_t getRip() {
			return get<uint64_t, 0xe8>();
		}

		TrapFrame &setRip(uint64_t rip) {
			set<0xe8>(rip);
			return *this;
		}
};
