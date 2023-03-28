#include "onix/console.h"
#include "onix/global.h"
#include "onix/interrupt.h"
#include "onix/clock.h"
#include "onix/task.h"
#include "onix/printk.h"

void kernel_init()
{
    console_init();
    gdt_init();
    interrupt_init();
    clock_init();
    printk("\a\n");
    task_init();
    return;
}
