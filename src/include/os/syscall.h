#ifndef OS_SYSCALL_H
#define OS_SYSCALL_H

#include <os/types.h>

typedef enum syscall_t
{
    SYS_NR_TEST = 0,
    SYS_NR_EXIT = 1,
    SYS_NR_FORK = 2,
    SYS_NR_WRITE = 4,
    SYS_NR_WAITPID = 7,
    SYS_NR_TIME = 13,
    SYS_NR_GETPID = 20,
    SYS_NR_BRK = 45,
    SYS_NR_GETPPID = 64,
    SYS_NR_YIELD = 158,
    SYS_NR_SLEEP = 162,
} syscall_t;

uint32_t test();
void exit(int status);
pid_t waitpid(pid_t pid, int32_t *status);
pid_t fork();
void yield();
void sleep(uint32_t ms);
int32_t brk(void *addr);
int32_t write(fd_t fd, char *buf, uint32_t len);

pid_t getpid();
pid_t getppid();

time_t time();

#endif