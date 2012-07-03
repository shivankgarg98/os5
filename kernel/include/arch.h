#pragma once

#ifndef __ASSEMBLER__

#define outb(port, val) \
	asm volatile ("outb %%al, %0" : : "dN" ((uint16_t)port), "a" ((uint16_t)val))

#define outw(port, val) \
	asm volatile ("outw %1, %0" : : "dN" ((uint16_t)port), "a" ((uint16_t)val))

#define inb(port) ({ \
	uint8_t __ret; \
	asm volatile ("inb %1, %0" : "=a" (__ret) : "dN" ((uint16_t)port)); \
	__ret; })

#define inw(port) ({ \
	uint16_t __ret; \
	asm volatile ("inw %1, %0" : "=a" (__ret) : "dN" ((uint16_t)port)); \
	__ret; })

#define enable_interrupts() asm("sti")

#define disable_interrupts() asm("cli");

typedef struct
{
uint32_t ds;
uint32_t edi, esi, ebp, esp;
uint32_t ebx, edx, ecx, eax;
uint32_t int_no, err_code;
uint32_t eip;
uint32_t cs;
uint32_t eflags, useresp, ss;
} registers;

#endif

#ifdef __ASSEMBLER__

%macro SetSegments 2
	mov e%2, %1
	mov ds, %2
	mov es, %2
	mov fs, %2
	mov gs, %2
	mov ss, %2
%endmacro

#endif