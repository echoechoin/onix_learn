#ifndef ONIX_MEMORY_H
#define ONIX_MEMORY_H
#include "onix/onix.h"
#include "onix/types.h"
#include "onix/debug.h"
#include "onix/types.h"
#include "onix/assert.h"

#define PAGE_SIZE 0x1000     // 一页的大小 4K
#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置

void memory_init(uint32 magic, uint32 addr);

#endif