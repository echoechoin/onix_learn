#include "onix/syscall.h"
#include "onix/types.h"

static uint32 _syscall0(uint32 nr)
{
    uint32 ret;
    asm volatile (
        "movl %1, %%eax\n\t" 
        "int $0x80\n\t" 
        "movl %%eax, %0\n\t" 
        : "=r" (ret) 
        : "r" (nr)
        : "%eax"
    );
    return ret;
}

static uint32 _syscall1(uint32 nr, uint32 arg)
{
    uint32 ret;
    asm volatile(
        "movl %1, %%eax\n\t"
        "int $0x80\n"
        "movl %%eax, %0\n\t"
        : "=r" (ret)
        : "r" (nr), "b" (arg)
        : "%eax"
    );
    return ret;
}

uint32 test()
{
    return _syscall0(SYS_NR_TEST);
}

void yield()
{
    _syscall0(SYS_NR_YIELD);
}

void sleep(uint32 ms)
{
    _syscall1(SYS_NR_SLEEP, ms);
}