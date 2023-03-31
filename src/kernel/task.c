#include "onix/onix.h"
#include "onix/task.h"
#include "onix/printk.h"
#include "onix/debug.h"
#include "onix/types.h"
#include "onix/task.h"
#include "onix/bitmap.h"
#include "onix/memory.h"
#include "onix/interrupt.h"
#include "onix/string.h"

extern bitmap_t kernel_map;
extern void task_switch(task_t *next);

#define N_TASKS 64
// 任务PCB数组
static task_t *task_table[N_TASKS] = {0};
// 任务默认阻塞链表
static list_t block_list;

static task_t *get_free_task()
{
    for (int i = 0; i < N_TASKS; i++)
    {
        if (task_table[i] == NULL)
        {
            // 每个任务栈大小为1页，栈起始地址存放task_t结构体
            task_table[i] = (task_t *)alloc_kpage(1);
            return task_table[i];
        }
    }
    return NULL;
}

// 从任务列表中查找某种状态的任务，处理自己
static task_t *task_search(task_state_t state)
{
    assert(!get_interrupt_state());
    task_t *task = NULL;
    // 获取当前任务PCB
    task_t *current = running_task();

    for (size_t i = 0; i < N_TASKS; i++)
    {
        task_t *ptr = task_table[i];
        if (ptr == NULL)
            continue;
        
        if (ptr->state == !state)
            continue;

        if (current == ptr)
            continue;

        // 该判断条件是为了寻找一个等待时间 ticks 长、运行时间 jiffies 短、
        // 并且从就绪状态开始就等待时间长的任务。这样的任务已经等待了较长时间，
        // 优先调度它可以提高任务的响应速度。
        if (task == NULL || task->ticks < ptr->ticks || ptr->jiffies < task->jiffies)
            task = ptr;
    }
    return task;
}

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
    // 切换到下一个准备好的任务
    task_t *next = task_search(TASK_READY);

    // 说明只有一个任务在运行
    if (next == NULL)
        return;

    // 防止栈溢出
    assert(next->magic == ONIX_MAGIC);

    if (current->state == TASK_RUNNING)
        current->state = TASK_READY;

    next->state = TASK_RUNNING;
    if (next == current)
        return;

    task_switch(next);
}

// 创建内核线程
static task_t *task_create(target_t target, const char *name, uint32 priority, uint32 uid)
{
    task_t *task = get_free_task();
    if (task == NULL)
        printk("task_create(%s): no free task\n", name);

    memset(task, 0, sizeof(task_t));
    // 栈顶指针
    uint32 stack = (uint32)task + PAGE_SIZE;

    // 将task_frame_t结构体放在栈顶
    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;

    // 存储一些无关的值到frame中，方便后续的调试
    frame->edi = 0x11111111;
    frame->esi = 0x22222222;
    frame->ebx = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)target;

    strncpy(task->name, name, TASK_NAME_LEN - 1);
    task->name[TASK_NAME_LEN - 1] = '\0';

    task->stack = (uint32 *)stack;
    task->priority = priority;
    task->ticks = priority;
    task->jiffies = 0;
    task->state = TASK_READY;
    task->uid = uid;
    task->vmap = &kernel_map;
    task->magic = ONIX_MAGIC;
    return task;
}

// 初始化最开始的任务，这个任务不会保存到任务列表中
static void task_setup()
{
    task_t *task = running_task();
    task->magic = ONIX_MAGIC;
    task->ticks = 1;
    strncpy(task->name, "main", TASK_NAME_LEN - 1);
    task->name[TASK_NAME_LEN - 1] = '\0';
    memset(task_table, 0, sizeof(task_table));
}

void task_yield()
{
    schedule();
}

// 任务阻塞
void task_block(task_t *task, list_t *blist, task_state_t state)
{
    assert(!get_interrupt_state());
    assert(task->node.next == NULL);
    assert(task->node.prev == NULL);

    if (blist == NULL)
    {
        blist = &block_list;
    }

    list_push(blist, &task->node);

    assert(state != TASK_READY && state != TASK_RUNNING);

    task->state = state;

    task_t *current = running_task();
    if (current == task)
    {
        schedule();
    }
}

// 解除任务阻塞
void task_unblock(task_t *task)
{
    assert(!get_interrupt_state());

    list_remove(&task->node);

    assert(task->node.next == NULL);
    assert(task->node.prev == NULL);

    task->state = TASK_READY;
}

uint32 thread_a()
{
    set_interrupt_state(true);
    while (true)
    {
        printk("thread: %s\n", running_task()->name);
    }
}

uint32 thread_b()
{
    set_interrupt_state(true);
    bool is_print = false;
    while (true)
    {
        if (!is_print)
            printk("thread: %s\n", running_task()->name);
        is_print = true;
    }
}

uint32 thread_c()
{
    set_interrupt_state(true);
    while (true)
    {
        printk("thread: %s\n", running_task()->name);
    }
}

void task_init()
{
    list_init(&block_list);
    task_setup();
    // task_create(thread_a, "a", 5, KERNEL_USER);
    task_create(thread_b, "b", 5, KERNEL_USER);
    // task_create(thread_c, "c", 5, KERNEL_USER);
}
