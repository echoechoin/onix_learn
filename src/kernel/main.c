#include "onix/memory.h"
#include "onix/interrupt.h"
#include "onix/clock.h"
#include "onix/time.h"
#include "onix/rtc.h"
#include "onix/task.h"
#include "onix/gate.h"
#include "onix/syscall.h"
#include "onix/keyboard.h"

void kernel_init()
{
    memory_map_init();
    mapping_init();
    interrupt_init();
    clock_init();
    time_init();
    rtc_init();
    syscall_init();
    // 初始化任务
    task_init();
    keyboard_init();

    // 启动中断！
    set_interrupt_state(true);
}
