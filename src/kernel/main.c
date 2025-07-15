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
extern void syscall_init();

extern void keyboard_init();

void test_syscall_0()
{
    asm volatile("mov $0, %eax\n");
    asm volatile("int $0x80"); // 虽然仍然还是在内核态调用的系统调用
}

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
    syscall_init();
    keyboard_init();
    set_interrupt_state(true);
    test_syscall_0();
    return;
}
