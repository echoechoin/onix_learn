#include "onix/debug.h"
#include "onix/stdarg.h"
#include "onix/stdio.h"
#include "onix/printk.h"

static uint8 buf[1024];

void debugk(char *file, int line, char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);

    printk("debug: %s:%d %s\n", file, line, buf);
}