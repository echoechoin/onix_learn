#include "os/stdlib.h"
#include "os/types.h"
#include <os/interrupt.h>
#include <os/syscall.h>
#include <os/debug.h>
#include <os/syscall.h>
#include <os/mutex.h>
#include <os/task.h>
#include <os/stdio.h>
#include <os/arena.h>

lock_t l;

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

void idle_thread()
{
    set_interrupt_state(true);
    uint32_t counter = 0;
    while (true)
    {
        if (++counter % 100 == 0)
            LOGK("idle task.... %d\n", counter);
        asm volatile(
            "sti\n" // 开中断
            "hlt\n" // 关闭 CPU，进入暂停状态，等待外中断的到来
        );
        yield(); // 放弃执行权，调度执行其他任务
    }
}

extern uint32_t keyboard_read(char *buf, uint32_t count);

void test_recursion()
{
    char tmp[0x400];
    test_recursion(); 
}

static void real_init_thread()
{
    uint32_t counter = 0;
    int status;
    char ch;
    while (true)
    {
        pid_t pid = fork();
        // test_recursion(); // 最终会导致缺页异常
        // printk("hello world!\n"); // 触发异常，因为无法再printk中的中断使能指令
        // printf("hello world!\n");
        
        if (pid != 0) {
            printf("parent: pid: %d\n, ppid: %d\n", getpid(), getppid());
            pid_t child = waitpid(pid, &status);
        } else {
            printf("child:  pid: %d\n, ppid: %d\n", getpid(), getppid());
            sleep(1000);
            exit(0);
        }
        sleep(1000);
    }
}

void init_thread()
{
    char temp[100]; // 为栈顶有足够的空间存储intr_frame
    task_to_user_mode(real_init_thread);
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
        // void *ptr = kmalloc(1200);
        // LOGK("kmalloc 0x%p....\n", ptr);
        // kfree(ptr);

        // ptr = kmalloc(1024);
        // LOGK("kmalloc 0x%p....\n", ptr);
        // kfree(ptr);

        // ptr = kmalloc(54);
        // LOGK("kmalloc 0x%p....\n", ptr);
        // kfree(ptr);

        sleep(1000);
    }
}