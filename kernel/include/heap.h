#pragma once
#include <stdint.h>

#ifndef __ASSEMBLER__
typedef struct heap_header_struct
{
	struct heap_header_struct *prev, *next;
	uint32_t allocated : 1;
	uint32_t size : 31;
} chunk_t;

void *kmalloc(uint32_t size);
void *kcalloc(uint32_t size);
void *kvalloc(uint32_t size);
void kfree(void *a);

#define chunk_head(a) ((chunk_t *)((uintptr_t)a-sizeof(chunk_t)))
#define chunk_data(c) ((void *)((uintptr_t)c + sizeof(chunk_t)))

#endif

