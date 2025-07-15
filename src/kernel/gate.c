#include <os/interrupt.h>
#include <os/assert.h>
#include <os/debug.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define SYSCALL_SIZE 64

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

extern void task_yield();
static void sys_yield()
{
    task_yield();
}

void syscall_init()
{
    for (size_t i = 0; i < SYSCALL_SIZE; i++)
    {
        syscall_table[i] = sys_default;
    }

    syscall_table[0] = sys_test;
    syscall_table[1] = sys_yield;
}
