#include <os/io.h>
#include <os/interrupt.h>
#include <os/assert.h>
#include <os/debug.h>
#include <os/clock.h>
#include <os/task.h>
#include <os/os.h>

// 时间片计数器
uint32_t volatile jiffies = 0;
uint32_t jiffy = JIFFY;  // jiffy 是瞬间的意思

void clock_handler(int vector)
{
    assert(vector == 0x20);
    send_eoi(vector);
    task_wakeup();

    jiffies++;

    task_t *task = running_task();
    assert(task->magic == OS_MAGIC);

    task->jiffies = jiffies;
    task->ticks--;
    if (!task->ticks)
    {
        task->ticks = task->priority;
        schedule();
    }
}

void pit_init()
{
    outb(PIT_CTRL_REG, 0b00110100);
    // 设置计数器的值：每隔 1 / 1193182 s 计数器减1；到0产生中断；自动重填计数器；
    outb(PIT_CHAN0_REG, CLOCK_COUNTER & 0xff);
    outb(PIT_CHAN0_REG, (CLOCK_COUNTER >> 8) & 0xff);
}

void clock_init()
{
    pit_init();
    set_interrupt_handler(IRQ_CLOCK, clock_handler);
    set_interrupt_mask(IRQ_CLOCK, true);
}
