#ifndef OS_MEMORY_H
#define OS_MEMORY_H

#include "os/types.h"

#define PAGE_SIZE 0x1000     // 一页的大小 4K
#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置

void memory_init(uint32_t magic, uint32_t addr);

#endif