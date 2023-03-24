#include "onix/task.h"
#include "onix/printk.h"
#include "onix/debug.h"

#define PAGE_SIZE 0x1000 // 4096

task_t *a = (task_t *)0x1000; // 表示第一个PCB所在的页地址为0x1000
task_t *b = (task_t *)0x2000; // 表示第二个PCB所在的页地址为0x2000

extern void task_switch(task_t *next);

task_t *running_task()
{   // 获取当前运行的进程的栈地址
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

uint32 thread_a()
{
    while (true)
    {
        printk("A");
        schedule();
    }
}

uint32 thread_b()
{
    while (true)
    {
        printk("B");
        schedule();
    }
}

static void task_create(task_t *task, target_t target)
{
    // 栈顶地址 = 栈底地址 + 栈大小（一页）
    uint32 stack = (uint32)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    // 栈顶存储一个task_frame_t结构体
    task_frame_t *frame = (task_frame_t *)stack;
    frame->ebx = 0x11111111;
    frame->esi = 0x22222222;
    frame->edi = 0x33333333;
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
