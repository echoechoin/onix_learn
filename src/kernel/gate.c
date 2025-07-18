#include "os/syscall.h"
#include <os/interrupt.h>
#include <os/assert.h>
#include <os/debug.h>
#include <os/console.h>
#include <os/memory.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define SYSCALL_SIZE 256

handler_t syscall_table[SYSCALL_SIZE];

void syscall_check(uint32_t nr)
{
    if (nr >= SYSCALL_SIZE)
    {
        panic("syscall nr error!!!");
    }
}

static void sys_default()
{
    panic("syscall not implemented!!!");
}

#include <os/task.h>
task_t *task = NULL;

static uint32_t sys_test()
{
    // LOGK("syscall test...\n");

    if (!task)
    {
        task = running_task();
        // LOGK("block task 0x%p \n", task);
        task_block(task, NULL, TASK_BLOCKED);
    }
    else
    {
        task_unblock(task);
        // LOGK("unblock task 0x%p \n", task);
        task = NULL;
    }

    return 255;
}


int32_t sys_write(fd_t fd, char *buf, uint32_t len)
{
    if (fd == stdout || fd == stderr)
    {
        return console_write(buf, len);
    }
    // todo
    panic("write!!!!");
    return 0;
}

void syscall_init()
{
    for (size_t i = 0; i < SYSCALL_SIZE; i++)
    {
        syscall_table[i] = sys_default;
    }

    syscall_table[SYS_NR_TEST] = sys_test;
    syscall_table[SYS_NR_FORK] = task_fork;
    syscall_table[SYS_NR_YIELD] = task_yield;
    syscall_table[SYS_NR_SLEEP] = task_sleep;
    syscall_table[SYS_NR_WRITE] = sys_write;
    syscall_table[SYS_NR_BRK] = sys_brk;
    syscall_table[SYS_NR_GETPID] = sys_getpid;
    syscall_table[SYS_NR_GETPPID] = sys_getppid;
}
