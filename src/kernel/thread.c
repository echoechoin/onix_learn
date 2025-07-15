#include <os/interrupt.h>
#include <os/syscall.h>
#include <os/debug.h>
#include <os/syscall.h>
#include <os/mutex.h>

lock_t l;

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

void idle_thread()
{
    set_interrupt_state(true);
    uint32_t counter = 0;
    while (true)
    {
        if (counter % 100 == 0)
            LOGK("idle task.... %d\n", counter++);
        asm volatile(
            "sti\n" // 开中断
            "hlt\n" // 关闭 CPU，进入暂停状态，等待外中断的到来
        );
        yield(); // 放弃执行权，调度执行其他任务
    }
}

extern uint32_t keyboard_read(char *buf, uint32_t count);

void init_thread()
{
    lock_init(&l);
    set_interrupt_state(true);

    char ch;
    while (true)
    {
        bool intr = interrupt_disable();
        keyboard_read(&ch, 1);
        // LOGK("%c\n", ch);
        printk("%c", ch);

        set_interrupt_state(intr);

        // LOGK("init task %d....\n", counter++);
        // sleep(500);
    }
}

void test_thread()
{
    set_interrupt_state(true);
    uint32_t counter = 0;

    while (true)
    {
        // lock_acquire(&l);
        // LOGK("test task %d....\n", counter++);
        // lock_release(&l);
        sleep(709);
    }
}