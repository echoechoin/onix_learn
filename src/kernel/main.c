#include "os/os.h"
#include "os/console.h"
#include "os/printk.h"
#include "os/assert.h"
#include "os/global.h"
#include "os/stdlib.h"
#include "os/task.h"
#include "os/interrupt.h"
#include "os/clock.h"
#include "os/rtc.h"
#include "os/time.h"

void kernel_init()
{
    console_init();
    gdt_init();
    show_gdt_list();
    // task_init();
    interrupt_init();
    clock_init();
    rtc_init();
    time_init();
    sti();
    return;
}
