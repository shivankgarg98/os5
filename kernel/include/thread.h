#pragma once

#include <common.h>
#include <arch.h>
#include <pmm.h>
#include <lists.h>
#include <idt.h>

#ifndef __ASSEMBLER__

struct process_struct;

typedef struct thread_struct
{
  registers_t r;
  uint32_t tid;
  uint32_t state;
  list_t tasks;
  list_t process_threads;
  registers_t *kernel_thread;
  struct process_struct *proc;
  list_t waiting;
} thread_t;

#define THREAD_STATE_READY 0x1
#define THREAD_STATE_WAITING 0x2
#define THREAD_STATE_FINISHED 0x3

// Changing this will require chaning kvalloc and all calls to it and current_thread_info()
#define MAX_THREAD_STACK_SIZE PAGE_SIZE
#define MIN_THREAD_STACK_SIZE (sizeof(uint32_t) * 100)

#define THREAD_STACK_SIZE (MAX_THREAD_STACK_SIZE - sizeof(thread_t) + sizeof(registers_t))
#define THREAD_STACK_SPACE (THREAD_STACK_SIZE - sizeof(registers_t))


typedef struct thread_info_struct
{
  uint8_t stackspace[THREAD_STACK_SPACE];
  thread_t tcb;
} thread_info_t;

thread_info_t *boot_thread;
uint32_t kernel_booted;

thread_info_t *current_thread_info();

#define current ((thread_t *)(&current_thread_info()->tcb))
#define stack_from_thinfo(info) ((uint32_t)&info->tcb.tid)
#define tcb_from_thinfo(info) ((thread_t *)(info->tcb))
#define thinfo_from_tcb(tcb) ((thread_info_t *)((uint32_t)(tcb)-THREAD_STACK_SPACE))
#define stack_from_tcb(tcb) (&tcb->tid)

thread_t *new_thread(void (*func)(void), uint8_t user);
void free_thread(thread_t *th);
registers_t *switch_kernel_thread(registers_t *r);
thread_t *clone_thread(thread_t *th);

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define schedule() __asm__ volatile("int $" TOSTRING(INT_SCHEDULE))

#endif
