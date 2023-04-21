#include "onix/thread.h"
#include "onix/interrupt.h"
#include "onix/syscall.h"
#include "onix/debug.h"
#include "onix/mutex.h"
#include "onix/keyboard.h"
#include "onix/printk.h"
#include "onix/task.h"

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

static void real_init_thread()
{
    uint32 counter = 0;

    char ch;
    while (true)
    {
        BMB;
        // asm volatile("in $0x92, %ax\n");
        // sleep(1000);
        // LOGK("%c\n", ch);
        // printk("%c", ch);
    }
}

void init_thread()
{
    // set_interrupt_state(true); 因为在进入用户态之前会设置iflag，所以这里不需要设置
    // 为栈顶有足够的空间，因为task_to_user_mode中有一些局部变量，但是intr_frame也要存储在栈顶位置，如果没有足够的空间，修改intr_frame也会修改到task_to_user_mode中的局部变量
    char temp[100];
    task_to_user_mode(real_init_thread);
}


void thread_test()
{
    set_interrupt_state(true);
    while (true)
    {
        LOGK("thread_test....\n");
        sleep(500);
    }
}