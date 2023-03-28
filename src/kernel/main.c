#include "onix/console.h"
#include "onix/global.h"
#include "onix/interrupt.h"
#include "onix/clock.h"
#include "onix/task.h"
#include "onix/printk.h"
#include "onix/time.h"

void kernel_init()
{
    console_init();
    gdt_init();
    interrupt_init();
    clock_init();
    time_init();
    task_init();
    return;
}
