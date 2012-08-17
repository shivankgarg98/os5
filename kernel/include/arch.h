#pragma once
#include <stdint.h>

#ifndef __ASSEMBLER__

// Functions for physical ports
// outb(port, val) - send byte val to port port
// outw(port, val) - send word val to port port
// inb(port) - read one byte from port port
// inw(port) - read one word from port port
#define outb(port, val) \
	__asm__ volatile ("outb %%al, %0" : : "dN" ((uint16_t)port), \
		"a" ((uint16_t)val))

#define outw(port, val) \
	__asm__ volatile ("outw %1, %0" : : "dN" ((uint16_t)port), \
		"a" ((uint16_t)val))

#define inb(port) ({ \
	uint8_t __ret; \
	__asm__ volatile ("inb %1, %0" : "=a" (__ret) : "dN" ((uint16_t)port)); \
	__ret; })

#define inw(port) ({ \
	uint16_t __ret; \
	__asm__ volatile ("inw %1, %0" : "=a" (__ret) : "dN" ((uint16_t)port)); \
	__ret; })


// Functions for turning interrupts on and off in the kernel.
#define enable_interrupts() __asm__("sti")
#define disable_interrupts() __asm__("cli");

// Storage space for registers as pushed to stack during interrupts
typedef struct
{
uint32_t ds;
uint32_t edi, esi, ebp, esp;
uint32_t ebx, edx, ecx, eax;
uint32_t int_no, err_code;
uint32_t eip;
uint32_t cs;
uint32_t eflags, useresp, ss;
} registers_t;

#define print_registers(r) \
	debug("\n\neax:%x ebx:%x ecx:%x edx:%x", \
		(r)->eax, (r)->ebx, (r)->ecx, (r)->edx); \
	debug("\nedi:%x esi:%x ebp:%x esp:%x", \
		(r)->edi, (r)->esi, (r)->ebp, (r)->esp); \
	debug("\neip:%x\ncs:%x ds:%x ss:%x", \
		(r)->eip, (r)->cs, (r)->ds, (r)->ss); \
	debug("\nuseresp:%x\neflags%x %b", \
		(r)->useresp, (r)->eflags, (r)->eflags); \
	debug("\nint_no:%x, err_code:%x %b", \
		(r)->int_no, (r)->err_code, (r)->err_code);

#endif

#ifdef __ASSEMBLER__
// Set all segment registers to the same value
// SetSegments val reg - set all segment registers to val using register reg

%macro SetSegments 2
	mov e%2, %1
	mov ds, %2
	mov es, %2
	mov fs, %2
	mov gs, %2
	mov ss, %2
%endmacro

#endif
