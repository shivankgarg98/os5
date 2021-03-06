#pragma once
#include <stdint.h>
#include <synch.h>
#include <serial.h>

#define VIDMEM  0xC00B8000

#define SCRN_W  80
#define SCRN_H  25

#define VGA_STYLE(text,bckg) text | bckg

#define BLACK 0x0
#define BLUE  0x01
#define GREEN 0x02
#define CYAN  0x03
#define RED   0x04
#define MAGENTA 0x05
#define BROWN 0x06
#define GRAY  0x07
#define LBLACK  0x08
#define LBLUE 0x09
#define LGREEN  0x0A
#define LCYAN 0x0B
#define LRED  0x0C
#define LMAGENTA  0x0D
#define LBROWN  0x0E
#define LGRAY 0x0F
#define WHITE 0x0F

#ifndef __ASSEMBLER__


void kdbg_init();
void kdbg_scroll();
void kdbg_putch(uint8_t c, uint8_t style);
void kdbg_setpos(uint32_t x, uint32_t y);
void kdbg_getpos(uint32_t *x, uint32_t *y);
void kdbg_puts(char *str);
void kdbg_printf(char *str, ...);

int kdbg_num2str(uint32_t num, uint32_t base, char *buf);
void kdbg_setclr(uint32_t style);

void print_stack_trace();

semaphore_t debug_sem;

#ifndef NDEBUG

/* #define debug kdbg_printf */
#define debug serial_debug
#define assert(n) ({if(!(n)){ \
  debug("\n WARNING! \n Assertion failed (%s)", #n); \
  debug(": %s line %d", __FILE__, __LINE__- 2); \
  for(;;);}})

#define panic(n) \
  ({ debug(n); \
  for(;;);})
#else

  #define debug(...) 
  #define assert(n) 

#endif

#endif

