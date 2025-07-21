#include <os/syscall.h>

// _syscall0 表示没有参数的系统调用
static _inline uint32_t _syscall0(uint32_t nr)
{
    uint32_t ret;
    asm volatile(
        "int $0x80\n"
        : "=a"(ret) // 将eax寄存器的值赋值给ret
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


static _inline uint32_t _syscall2(uint32_t nr, uint32_t arg1, uint32_t arg2)
{
    uint32_t ret;
    asm volatile(
        "int $0x80\n"
        : "=a"(ret)
        : "a"(nr), "b"(arg1), "c"(arg2));
    return ret;
}

static _inline uint32_t _syscall3(uint32_t nr, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    uint32_t ret;
    asm volatile(
        "int $0x80\n"
        : "=a"(ret)
        : "a"(nr), "b"(arg1), "c"(arg2), "d"(arg3));
    return ret;
}

uint32_t test()
{
    return _syscall0(SYS_NR_TEST);
}

void exit(int status)
{
    _syscall1(SYS_NR_EXIT, (uint32_t)status);
}

void yield()
{
    _syscall0(SYS_NR_YIELD);
}

void sleep(uint32_t ms)
{
    _syscall1(SYS_NR_SLEEP, ms);
}


int32_t write(fd_t fd, char *buf, uint32_t len)
{
    return _syscall3(SYS_NR_WRITE, fd, (uint32_t)buf, len);
}

pid_t getpid()
{
    return _syscall0(SYS_NR_GETPID);
}

pid_t getppid()
{
    return _syscall0(SYS_NR_GETPPID);
}

pid_t fork()
{
    return _syscall0(SYS_NR_FORK);
}

pid_t waitpid(pid_t pid, int32_t *status)
{
    return _syscall2(SYS_NR_WAITPID, pid, (uint32_t)status);
}

time_t time()
{
    return _syscall0(SYS_NR_TIME);
}
