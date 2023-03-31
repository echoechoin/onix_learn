#include "onix/memory.h"
#include "onix/interrupt.h"
#include "onix/clock.h"
#include "onix/time.h"
#include "onix/rtc.h"
#include "onix/task.h"
#include "onix/gate.h"
#include "onix/syscall.h"

void kernel_init()
{
    memory_map_init();
    mapping_init();
    interrupt_init();
    clock_init();
    time_init();
    rtc_init();
    syscall_init();
    // 初始化任务；启动中断，开始任务调度。
    task_init();
    set_interrupt_state(true);
    yield();
    test();
}
