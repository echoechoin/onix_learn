#include <os/syscall.h>

// _syscall0 表示没有参数的系统调用
static _inline uint32_t _syscall0(uint32_t nr)
{
    uint32_t ret;
    asm volatile(
        "int $0x80\n"
        : "a="(ret)
        : "a"(nr));
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
