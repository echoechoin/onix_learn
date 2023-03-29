#include "onix/console.h"
#include "onix/global.h"
#include "onix/interrupt.h"
#include "onix/clock.h"
#include "onix/task.h"
#include "onix/printk.h"
#include "onix/time.h"
#include "onix/rtc.h"
#include "onix/memory.h"

void kernel_init()
{
    memory_map_init();
    memory_test();
    gdt_init();
    interrupt_init();
    clock_init();
    time_init();
    rtc_init();
    task_init();
    return;
}
