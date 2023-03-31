#include "onix/stdarg.h"
#include "onix/console.h"
#include "onix/stdio.h"
#include "onix/printk.h"
#include "onix/interrupt.h"


static char buf[1024];

int printk(const char *fmt, ...)
{
    bool intr = interrupt_disable();

    va_list args;
    int i;

    va_start(args, fmt);

    i = vsprintf(buf, fmt, args);

    va_end(args);

    console_write(buf, i);

    set_interrupt_state(intr);
    return i;
}