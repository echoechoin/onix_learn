#include <os/task.h>
#include <os/printk.h>
#include <os/debug.h>
#include <os/memory.h>
#include <os/assert.h>
#include <os/interrupt.h>
#include <os/string.h>
#include <os/bitmap.h>
#include <os/os.h>

#define PAGE_SIZE 0x1000

task_t *a = (task_t *)0x1000;
task_t *b = (task_t *)0x2000;

extern bitmap_t kernel_map;
extern void task_switch(task_t *next);

#define NR_TASKS 64
static task_t *task_table[NR_TASKS];

// 从 task_table 里获得一个空闲的任务
static task_t *get_free_task()
{
    for (size_t i = 0; i < NR_TASKS; i++)
    {
        if (task_table[i] == NULL)
        {
            task_table[i] = (task_t *)alloc_kpage(1); // todo free_kpage
            return task_table[i];
        }
    }
    panic("No more tasks");
    return NULL;
}

// 从任务数组中查找某种状态的任务，自己除外
static task_t *task_search(task_state_t state)
{
    assert(!get_interrupt_state());
    task_t *task = NULL;
    task_t *current = running_task();

    for (size_t i = 0; i < NR_TASKS; i++)
    {
        task_t *ptr = task_table[i];
        if (ptr == NULL)
            continue;

        if (ptr->state != state)
            continue;
        if (current == ptr)
            continue;
        if (task == NULL || task->ticks < ptr->ticks || ptr->jiffies < task->jiffies)
            task = ptr;
    }

    return task;
}

task_t *running_task()
{
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n");
}

void schedule()
{
    task_t *current = running_task();
    task_t *next = task_search(TASK_READY);

    assert(next != NULL);
    assert(next->magic == OS_MAGIC);

    if (current->state == TASK_RUNNING)
        current->state = TASK_READY;

    if (!current->ticks)
        current->ticks = current->priority;

    next->state = TASK_RUNNING;
    if (next == current)
        return;

    task_switch(next);
}

uint32_t thread_a()
{
    while (true)
    {
        // printk("A");
        schedule();
    }
}

uint32_t thread_b()
{
    while (true)
    {
        // printk("B");
        schedule();
    }
}

static task_t *task_create(target_t target, const char *name, uint32_t priority, uint32_t uid)
{
    task_t *task = get_free_task();
    memset(task, 0, PAGE_SIZE);
    uint32_t stack = (uint32_t)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;
    frame->ebx = 0x11111111;
    frame->esi = 0x22222222;
    frame->edi = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)target;

    strcpy((char *)task->name, name);

    task->stack = (uint32_t *)stack;
    task->priority = priority;
    task->ticks = task->priority;
    task->jiffies = 0;
    task->state = TASK_READY;
    task->uid = uid;
    task->vmap = &kernel_map;
    task->pde = KERNEL_PAGE_DIR;
    task->magic = OS_MAGIC;

    return task;
}

static void task_setup()
{
    task_t *task = running_task();
    printk("task: %p\n", task);
    task->magic = OS_MAGIC;
    task->ticks = 1;

    memset(task_table, 0, sizeof(task_table));
}

void task_init()
{
    task_setup();
    task_create(thread_a, "a", 5, KERNEL_USER);
    task_create(thread_b, "b", 5, KERNEL_USER);
}
