#include <stdint.h>
#include <k_debug.h>
#include <pmm.h>
#include <vmm.h>
#include <multiboot.h>
#include <heap.h>
#include <idt.h>
#include <arch.h>
#include <memory.h>
#include <debug.h>
#include <thread.h>
#include <scheduler.h>
#include <timer.h>

void _idle(void)
{
		debug("A");
	for(;;)
	{
		__asm__ ("sti; hlt");
	}
}
void _clock(void)
{

	uint16_t data[128];
		debug("B");
	for(;;)
	{
		int i;
		for(i=0; i<128; i++)
		{
			disable_interrupts();
			outb(0x70,i);
			data[i] = inb(0x71);
			enable_interrupts();
		}
		uint16_t h = ((data[4]/16)*10 + (data[4]&0xf));
		uint16_t m = ((data[2]/16)*10 + (data[2]&0xf));
		uint16_t s = ((data[0]/16)*10 + (data[0]&0xf));
			
		uint32_t x,y;
		kdbg_getpos(&x,&y);
		kdbg_setpos(0,1);
		debug("[%d:%d:%d]",h,m,s);
		kdbg_setpos(x,y);
	}

}

registers_t *kinit(mboot_info_t *mboot, uint32_t mboot_magic)
{

	kdbg_init();
	pmm_init(mboot);
	idt_init();
	tss_init();
	scheduler_init();
	timer_init(500);

	register_int_handler(INT_PF, page_fault_handler);
	register_int_handler(INT_SCHEDULE, switch_kernel_thread);

	thread_t *idle = new_thread(&_idle,0);
	new_thread(&_clock,0);

	idle->r.eflags = EFL_INT;

	set_kernel_stack(stack_from_tcb(idle));


	return switch_kernel_thread(0);
}
