#include "onix/task.h"
#include "onix/printk.h"
#include "onix/debug.h"
#include "onix/types.h"

#define PAGE_SIZE 0x1000 // 4096

task_t *a = (task_t *)0x1000; // 表示第一个PCB所在的页地址为0x1000
task_t *b = (task_t *)0x2000; // 表示第二个PCB所在的页地址为0x2000

extern void task_switch(task_t *next);

task_t *running_task()
{   // 获取当前运行的进程的PCB
    // 原理就是通过读取esp寄存器的值与上一页的地址，就可以得到当前运行的进程的PCB
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n");
}

void schedule()
{
    task_t *current = running_task();
    // 切换到下一个进程
    task_t *next = current == a ? b : a;
    // 调用task_switch的时候eip会保存到栈中
    task_switch(next);
}

uint32 _ofp thread_a()
{
    asm volatile("sti\n");
    while (true)
    {
        // for (int i = 0; i < 100; i++)
        // {
        //     asm volatile("nop");
        // }
        // asm volatile("cli");
        // printk("A"); // 因为printk暂时还不是线程安全的，所以这里会出现问题，需要暂时禁用中断，保证原子性
        // asm volatile("sti\n");
    }
}

uint32 _ofp thread_b()
{
    asm volatile("sti\n");
    while (true)
    {
        // for (int i = 0; i < 100; i++)
        // {
        //     asm volatile("nop");
        // }
        // asm volatile("cli");
        // printk("B");
        // asm volatile("sti\n");
    }
}

static void task_create(task_t *task, target_t target)
{
    // 栈顶地址 = 栈底地址 + 栈大小（一页）
    uint32 stack = (uint32)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    // 栈顶存储一个task_frame_t结构体
    task_frame_t *frame = (task_frame_t *)stack;
    // 初始化的时候随便填写的数字
    frame->edi = 0x11111111;
    frame->esi = 0x22222222;
    frame->ebx = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)target;

    task->stack = (uint32 *)stack;
}

void task_init()
{
    task_create(a, thread_a);
    task_create(b, thread_b);
    schedule();
}
