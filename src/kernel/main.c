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
#include "os/memory.h"

extern void memory_test();
extern void memory_map_init();
extern void mapping_init();

extern void task_init();

void kernel_init(uint32_t magic, uint32_t addr)
{
    console_init();
    gdt_init();
    show_gdt_list();
    interrupt_init();
    clock_init();
    rtc_init();
    time_init();
    memory_init(magic, addr);
    memory_map_init();
    mapping_init();
    task_init();
    set_interrupt_state(true);
    return;
}
