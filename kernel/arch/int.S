
# This line is required since newlib is not assembler safe.
# Shame on you, newlib maintainers!
#define _STDINT_H

#include <arch.h>
#include <idt.h>

# Reload IDT
.global idt_flush
.func idt_flush
idt_flush:
  mov 4(%esp), %eax
  lidt (%eax)
  ret
.endfunc

# Reload TSS
.global tss_flush
.func tss_flush
tss_flush:
  mov 4(%esp), %eax
  ltr %ax
  ret
.endfunc

# ISR stubs
INTNOERR 0
INTNOERR 1
INTNOERR 2
INTNOERR 3
INTNOERR 4
INTNOERR 5
INTNOERR 6
INTNOERR 7
INTERR 8
INTNOERR 9
INTERR 10
INTERR 11
INTERR 12
INTERR 13
INTERR 14
INTNOERR 15
INTNOERR 16
INTNOERR 17
INTNOERR 18
INTNOERR 19
INTNOERR 20
INTNOERR 21
INTNOERR 22
INTNOERR 23
INTNOERR 24
INTNOERR 25
INTNOERR 26
INTNOERR 27
INTNOERR 28
INTNOERR 29
INTNOERR 30
INTNOERR 31
INTNOERR 32
INTNOERR 33
INTNOERR 34
INTNOERR 35
INTNOERR 36
INTNOERR 37
INTNOERR 38
INTNOERR 39
INTNOERR 40
INTNOERR 41
INTNOERR 42
INTNOERR 43
INTNOERR 44
INTNOERR 45
INTNOERR 46
INTNOERR 47

INTNOERR 128
INTNOERR 255
INTNOERR 130

.extern idt_handler

# Common ISR stub
# Push all registers to stack and jump to c
.global int_stub
.func int_stub
int_stub:
  pusha
  xor %ecx, %ecx
  mov %ds, %cx
  push %ecx

  SetSegments $0x10, cx

  push %esp
  call idt_handler
  mov %eax, %esp
.endfunc

# Return from interrupt
# Pop all registers and perform iret
.global int_return
.func int_return
int_return:
  pop %eax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %fs
  mov %ax, %gs

  popa
  add $8, %esp

  iret
.endfunc

