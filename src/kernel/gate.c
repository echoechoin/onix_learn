#include "onix/interrupt.h"
#include "onix/assert.h"
#include "onix/debug.h"
#include "onix/syscall.h"
#include "onix/task.h"

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define SYSCALL_SIZE 64

handler_t syscall_table[SYSCALL_SIZE];

// 判断系统调用号是否合法
void syscall_check(uint32 nr)
{
    if (nr >= SYSCALL_SIZE)
    {
        panic("syscall nr error!!!");
    }
}

// 初始系统调用处理函数
static void sys_default()
{
    panic("syscall not implemented!!!");
}

static uint32 sys_test()
{
    LOGK("syscall test...\n");
    return 255;
}

// 初始化系统调用表
void syscall_init()
{
    for (size_t i = 0; i < SYSCALL_SIZE; i++)
    {
        syscall_table[i] = sys_default;
    }

    syscall_table[SYS_NR_TEST] = sys_test;
    syscall_table[SYS_NR_YIELD] = task_yield;
    syscall_table[SYS_NR_SLEEP] = task_sleep;
}
