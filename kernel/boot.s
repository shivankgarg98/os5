
%include "include/asm_macros.inc"

[bits 32]

section .bss

align 0x8

; Stack for booting
[global BootStack]
BootStackTop:
	resb BOOT_STACK_SIZE
BootStack:

section .data

align 0x1000

; Page directory for booting up.
; First four megabytes are identity mapped as well as
; mapped to 0xC0000000
[global BootPageDirectory]
BootPageDirectory:
	dd (BootPageTable - KERNEL_OFFSET) + 0x3
	times ((KERNEL_OFFSET >> 22) - 1) dd 0x0
	dd (BootPageTable - KERNEL_OFFSET) + 0x3
	times (1022 - (KERNEL_OFFSET >> 22)) dd 0x0
	dd (BootPageDirectory - KERNEL_OFFSET) + 0x3

BootPageTable:
	%assign i 0
	%rep 1024
		dd (i << 12) | 0x3
		%assign i i+1
	%endrep

; Hard-coded GDT.
; GDT pointer is wrapped into the first entry
[global gdt]
gdt_ptr:
gdt:
	dw 0x002F
	dd gdt
	dw 0x0000
	dd 0x0000FFFF, 0x00CF9A00
	dd 0x0000FFFF, 0x00CF9200
	dd 0x0000FFFF, 0x00CFFA00
	dd 0x0000FFFF, 0x00CFF200

section .text

align 4

; GRUB Multiboot data
MultiBootHeader:
	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_HEADER_CHECKSUM

; Kernel start point
[global start]
start:
	cli

; Load page directory and enable paging
	mov ecx, BootPageDirectory - KERNEL_OFFSET
	mov cr3, ecx
	mov ecx, cr0
	or ecx, 0x80000000
	mov cr0, ecx
	lea ecx, [.higherHalf]
	jmp ecx

.higherHalf:
	; Load GDT
	mov ecx, gdt_ptr
	lgdt [ecx]

	SetSegments 0x10, cx
	jmp 0x8:.gdtLoaded

.gdtLoaded:
	; Clear the identity mapping from the page directory
	mov edx, BootPageDirectory
	xor ecx, ecx
	mov [edx], ecx
	invlpg[0]

	; Load a stack for booting
	mov esp, BootStack
	mov ebp, BootStack

	; eax contains the magic number from GRUB 0x1BADB003
	push eax

	; ebx contains the address of the Multiboot information structure
	add ebx, KERNEL_OFFSET
	push ebx

	; Call the c function for setting up
[extern kinit]
call kinit
jmp $
