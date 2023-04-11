#include "onix/thread.h"
#include "onix/interrupt.h"
#include "onix/syscall.h"
#include "onix/debug.h"
#include "onix/mutex.h"
#include "onix/keyboard.h"
#include "onix/printk.h"

void idle_thread()
{
    set_interrupt_state(true);
    while (true)
    {
        asm volatile(
            "sti\n" // 开中断
            "hlt\n" // 关闭 CPU，进入暂停状态，等待外中断的到来
        );
        yield(); // 放弃执行权，调度执行其他任务
    }
}

mutex_t mutex;

void init_thread()
{
    mutex_init(&mutex);
    set_interrupt_state(true);
    mutex_lock(&mutex);

    char ch;
    while (true)
    {
        bool intr = interrupt_disable();
        keyboard_read(&ch, 1);
        set_interrupt_state(intr);
        printk("%c", ch);
    }
    mutex_unlock(&mutex);
}

void thread_test()
{
    set_interrupt_state(true);
    while (true)
    {
        mutex_lock(&mutex); // 这里永远也得不到互斥量
        LOGK("thread_test....\n");
        sleep(500);
        mutex_unlock(&mutex);
    }
}