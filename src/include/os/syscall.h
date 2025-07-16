#ifndef OS_SYSCALL_H
#define OS_SYSCALL_H

#include <os/types.h>

typedef enum syscall_t
{
    SYS_NR_TEST = 0,
    SYS_NR_YIELD,
    SYS_NR_SLEEP,
    SYS_NR_WRITE,
} syscall_t;

uint32_t test();
void yield();
void sleep(uint32_t ms);
int32_t write(fd_t fd, char *buf, uint32_t len);

#endif