#include "onix/memory.h"
#include "onix/interrupt.h"
#include "onix/clock.h"
#include "onix/time.h"
#include "onix/rtc.h"
#include "onix/task.h"

void kernel_init()
{
    memory_map_init();
    mapping_init();
    interrupt_init();
    clock_init();
    time_init();
    rtc_init();
    // task_init();
}
