#ifndef ONIX_SYSCALL_H
#define ONIX_SYSCALL_H

#include <onix/types.h>

typedef enum syscall_t
{
    SYS_NR_TEST,
    SYS_NR_YIELD,
    SYS_NR_SLEEP,
} syscall_t;

uint32 test();
void yield();
void sleep(uint32 ms);

#endif