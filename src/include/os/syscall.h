#ifndef OS_SYSCALL_H
#define OS_SYSCALL_H

#include <os/types.h>

typedef enum syscall_t
{
    SYS_NR_TEST = 0,
    SYS_NR_YIELD,
} syscall_t;

uint32_t test();
void yield();

#endif