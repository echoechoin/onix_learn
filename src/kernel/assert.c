#include "onix/assert.h"
#include "onix/types.h"
#include "onix/stdio.h"
#include "onix/printk.h"

static uint8 buf[1024];

// 阻塞
static void spin(char *name)
{
    printk("sping in %s ...\n", name);
    while (1) {
        __asm__ __volatile__("hlt");
    }
}

void assertion_failure(char *exp, char *file, char *base, int line)
{
    printk(
        "--> assertion failure: %s\n"
        "--> file: %s\n"
        "--> base: %s\n"
        "--> line: %d\n",
        exp, file, base, line);

    spin("assertion_failure()");

    __asm__ __volatile__("ud2");
}

void panic(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    printk("panic: %s\n", buf);

    spin("panic()");

    __asm__ __volatile__("ud2");
}
