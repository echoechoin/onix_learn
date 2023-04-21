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
#include "onix/thread.h"
#include "onix/global.h"

extern bitmap_t kernel_map;
extern void task_switch(task_t *next);
extern uint32 volatile jiffies;
extern uint32 jiffy;
extern tss_t tss;


#define N_TASKS 64
// 任务PCB数组
static task_t *task_table[N_TASKS] = {0};
// 任务默认阻塞链表
static list_t block_list;
// 任务睡眠队列
static list_t sleep_list;
// idle task PCB
static task_t *idle_task;
// init task PCB
static task_t *init_task;

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
        
        if (ptr->state != state)
            continue;

        if (current == ptr)
            continue;

        // 该判断条件是为了寻找一个等待时间 ticks 长、运行时间 jiffies 短、
        // 并且从就绪状态开始就等待时间长的任务。这样的任务已经等待了较长时间，
        // 优先调度它可以提高任务的响应速度。
        if (task == NULL || task->ticks < ptr->ticks || ptr->jiffies < task->jiffies)
            task = ptr;
    }

    // 没有找到READY状态的任务，返回idle_task
    if (task == NULL && state == TASK_READY)
    {
        task = idle_task;
    }

    return task;
}

task_t *running_task()
{   // 获取当前运行的进程的PCB, 也就是任务栈的起始地址
    // 原理就是通过读取esp寄存器的值与上一页的地址，就可以得到当前运行的进程的PCB
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n");
}

// 激活任务：切换到用户态任务的话需要保存当前的esp0到tss中
void task_activate(task_t *task)
{
    assert(task->magic == ONIX_MAGIC);

    if (task->uid != KERNEL_USER)
    {
        tss.esp0 = (uint32)task + PAGE_SIZE;
    }
}

void schedule()
{
    task_t *current = running_task();
    // 切换到下一个准备好的任务
    task_t *next = task_search(TASK_READY);

    // 总归是能找到idle task的
    assert(next != NULL);

    // 防止栈溢出
    assert(next->magic == ONIX_MAGIC);

    if (current->state == TASK_RUNNING)
        current->state = TASK_READY;

    next->state = TASK_RUNNING;
    if (next == current)
        return;

    task_activate(next);
    task_switch(next); // 这里会将eip压入栈中
}

void task_to_user_mode(target_t target)
{
    task_t *task = running_task();

    // 在栈顶构造中断帧，这样当中断返回时，就会进入用户态
    uint32 addr = (uint32)task + PAGE_SIZE;
    addr -= sizeof(intr_frame_t);
    intr_frame_t *iframe = (intr_frame_t *)addr;

    // 保存一些无关的值到frame中，方便后续的调试
    iframe->vector = 0x20;
    iframe->edi = 1;
    iframe->esi = 2;
    iframe->ebp = 3;
    iframe->esp_dummy = 4;
    iframe->ebx = 5;
    iframe->edx = 6;
    iframe->ecx = 7;
    iframe->eax = 8;

    // 设置段寄存器
    iframe->gs = 0;
    iframe->ds = USER_DATA_SELECTOR;
    iframe->es = USER_DATA_SELECTOR;
    iframe->fs = USER_DATA_SELECTOR;
    iframe->cs = USER_CODE_SELECTOR;
    iframe->ss3 = USER_DATA_SELECTOR; // 用户态栈段选择子

    iframe->error = ONIX_MAGIC;

    // 目前用户态和内核态共用一个页表
    uint32 stack3 = alloc_kpage(1); // todo replace to user stack

    iframe->eip = (uint32)target;
    // IF位为1，允许中断，用于任务调度和系统调用
    // IO previlege level 为 0，不允许用户态访问 IO：in和out指令会在用户态发生异常
    iframe->eflags = (0 << 12 | 1 << 9);
    // 设置用户态栈顶
    iframe->esp3 = stack3 + PAGE_SIZE;

    // 设置栈顶指针，恢复用户态中断帧（也就是上面我们存储的中断帧iframe）
    asm volatile("movl %0, %%esp"::"m"(iframe));
    asm volatile("addl $4, %esp"); // 跳过vector
    asm volatile("popal"); // 从栈中弹出寄存器
    asm volatile("pop %gs");
    asm volatile("pop %fs");
    asm volatile("pop %es");
    asm volatile("pop %ds");
    asm volatile("addl $8, %esp");
    asm volatile("iret");
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

// 任务睡眠
void task_sleep(uint32 ms)
{
    // 由于是从中断门进入的，所以eflags的IF位肯定得是0
    assert(!get_interrupt_state());

    // 需要将ms转换为ticks
    uint32 ticks = ms / jiffy;
    // 至少需要睡眠1个ticks
    ticks = ticks > 0 ? ticks : 1;

    task_t *current = running_task();
    // 记录需要唤醒得时间
    current->ticks = jiffies + ticks;

    // 将当前任务加入到睡眠队列中
    // 且该队列是按照ticks从大到小排序的
    // 其实使用红黑树更好？
    list_t *list = &sleep_list;
    list_node_t *anchor = &list->tail;
    for (list_node_t *ptr = list->head.next; ptr != &list->tail; ptr = ptr->next)
    {
        task_t *task = element_entry(task_t, node, ptr);

        if (task->ticks > current->ticks)
        {
            anchor = ptr;
            break;
        }
    }
    assert(current->node.next == NULL);
    assert(current->node.prev == NULL);
    list_insert_before(anchor, &current->node);

    // 设置为睡眠状态
    current->state = TASK_SLEEPING;

    // 切换到下一个任务
    schedule();
}

void task_wakeup()
{
    assert(!get_interrupt_state());

    list_t *list = &sleep_list;
    list_node_t *ptr = list->head.next;
    while (ptr != &list->tail)
    {
        task_t *task = element_entry(task_t, node, ptr);
        ptr = ptr->next;

        if (task->ticks <= jiffies)
        {
            list_remove(&task->node);
            task->state = TASK_READY;
        }
    }
}

// uint32 thread_a()
// {
//     set_interrupt_state(true);
//     while (true)
//     {
//         printk("thread: %s\n", running_task()->name);
//     }
// }

// uint32 thread_b()
// {
//     set_interrupt_state(true);
//     bool is_print = false;
//     while (true)
//     {
//         if (!is_print)
//             printk("thread: %s\n", running_task()->name);
//         is_print = true;
//     }
// }



void task_init()
{
    list_init(&block_list);
    list_init(&sleep_list);
    task_setup();
    // 因为idle task啥也不做，所以把优先级设置为最低
    idle_task = task_create(idle_thread, "idle", 1, KERNEL_USER);
    init_task = task_create(init_thread, "init", 5, NORMAL_USER);
    task_create(thread_test, "thread test", 5, NORMAL_USER);
}
