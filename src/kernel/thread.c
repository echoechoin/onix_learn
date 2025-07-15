#include <os/interrupt.h>
#include <os/syscall.h>
#include <os/debug.h>
#include <os/syscall.h>
#include <os/mutex.h>

mutex_t mutex;

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

void idle_thread()
{
    set_interrupt_state(true);
    uint32_t counter = 0;
    while (true)
    {
        LOGK("idle task.... %d\n", counter++);
        asm volatile(
            "sti\n" // 开中断
            "hlt\n" // 关闭 CPU，进入暂停状态，等待外中断的到来
        );
        yield(); // 放弃执行权，调度执行其他任务
    }
}

void init_thread()
{
    mutex_init(&mutex);
    set_interrupt_state(true);

    while (true)
    {
        mutex_lock(&mutex);
        // LOGK("init task....\n");
        mutex_unlock(&mutex);
        // test();
    }
}

void test_thread()
{
    set_interrupt_state(true);
    uint32_t counter = 0;

    while (true)
    {
        mutex_lock(&mutex);
        LOGK("test task %d....\n", counter++);
        mutex_unlock(&mutex);
        sleep(709);
    }
}