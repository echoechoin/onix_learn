#include <os/syscall.h>

// _syscall0 表示没有参数的系统调用
static _inline uint32_t _syscall0(uint32_t nr)
{
    uint32_t ret;
    asm volatile(
        "int $0x80\n"
        : "=a"(ret)
        : "a"(nr));
    return ret;
}

// _syscall1 表示有一个参数的系统调用
static _inline uint32_t _syscall1(uint32_t nr, uint32_t arg)
{
    uint32_t ret;
    asm volatile(
        "int $0x80\n"
        : "=a"(ret)
        : "a"(nr), "b"(arg));
    return ret;
}

uint32_t test()
{
    return _syscall0(SYS_NR_TEST);
}

void yield()
{
    _syscall0(SYS_NR_YIELD);
}

void sleep(uint32_t ms)
{
    _syscall1(SYS_NR_SLEEP, ms);
}
