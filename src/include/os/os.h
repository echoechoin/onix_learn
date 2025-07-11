#ifndef OS_H
#define OS_H
#include "os/types.h"

#define OS_MAGIC 0xdeadbeef

void kernel_init(uint32_t magic, uint32_t addr); //初始化内核

#endif
