#include "os/syscall.h"
#include <os/task.h>
#include <os/printk.h>
#include <os/debug.h>
#include <os/memory.h>
#include <os/assert.h>
#include <os/interrupt.h>
#include <os/string.h>
#include <os/bitmap.h>
#include <os/os.h>
#include <os/list.h>
#include <os/clock.h>
#include <os/global.h>
#include <os/syscall.h>
#include <os/arena.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define PAGE_SIZE 0x1000

extern uint32_t volatile jiffies;
extern uint32_t jiffy;
extern bitmap_t kernel_map;
extern void task_switch(task_t *next);
extern tss_t tss;

#define NR_TASKS 64
static task_t *task_table[NR_TASKS]; // 任务表
static list_t block_list;            // 任务默认阻塞链表
static list_t sleep_list;            // 任务睡眠链表
static task_t *idle_task;


// 从 task_table 里获得一个空闲的任务
static task_t *get_free_task()
{
    for (size_t i = 0; i < NR_TASKS; i++)
    {
        if (task_table[i] == NULL)
        {
            task_t *task = (task_t *)alloc_kpage(1); // todo free_kpage
            memset(task, 0, PAGE_SIZE);
            task->pid = i;
            task_table[i] = task;
            return task;
        }
    }
    panic("No more tasks");
    return NULL;
}

// 获取进程 id
pid_t sys_getpid()
{
    task_t *task = running_task();
    return task->pid;
}

// 获取父进程 id
pid_t sys_getppid()
{
    task_t *task = running_task();
    return task->ppid;
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

    if (task == NULL && state == TASK_READY)
    {
        task = idle_task;
    }

    return task;
}

task_t *running_task()
{
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n");
}

// 激活任务
void task_activate(task_t *task)
{
    assert(task->magic == OS_MAGIC);

    // 切换到用户态前保存内核栈指针
    if (task->uid != KERNEL_USER)
    {
        tss.esp0 = (uint32_t)task + PAGE_SIZE;
    }

    // 修改任务的页目录
    if (task->pde != get_cr3())
    {
        set_cr3(task->pde);
        // BMB;
    }
}

void schedule()
{
    assert(!get_interrupt_state()); // 不可中断
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

    task_activate(next);
    task_switch(next);
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


void task_sleep(uint32_t ms)
{
    assert(!get_interrupt_state()); // 不可中断

    uint32_t ticks = ms / jiffy;        // 需要睡眠的时间片
    ticks = ticks > 0 ? ticks : 1; // 至少休眠一个时间片

    // 记录目标全局时间片，在那个时刻需要唤醒任务
    task_t *current = running_task();
    current->ticks = jiffies + ticks;

    // 从睡眠链表找到第一个比当前任务唤醒时间点更晚的任务，进行插入排序
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

    // 插入链表
    list_insert_before(anchor, &current->node);

    // 阻塞状态是睡眠
    current->state = TASK_SLEEPING;

    // 调度执行其他任务
    schedule();
}

void task_wakeup()
{
    assert(!get_interrupt_state()); // 不可中断

    // 从睡眠链表中找到 ticks 小于等于 jiffies 的任务，恢复执行
    list_t *list = &sleep_list;
    for (list_node_t *ptr = list->head.next; ptr != &list->tail;)
    {
        task_t *task = element_entry(task_t, node, ptr);
        if (task->ticks > jiffies)
        {
            break;
        }

        // unblock 会将指针清空
        ptr = ptr->next;

        task->ticks = 0;
        task_unblock(task);
    }
}


/**
 * 一个任务使用一页内存（4K）：
 *
 * +------------+ 4K
 * | task_frame |
 * +------------+ <- eip
 * |            |
 * |     ...    |
 * |            |
 * +------------+
 * |    task    | 
 * +------------- 0
 */


static task_t *task_create(target_t target, const char *name, uint32_t priority, uint32_t uid)
{
    task_t *task = get_free_task();
    uint32_t stack = (uint32_t)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;
    // 构造一个栈帧，task_switch的时候会被恢复
    frame->ebx = 0x11111111; // 因为刚进入task的时候这些寄存器的值是不确定的，所以随便填个值用于调试
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
    task->brk = KERNEL_MEMORY_SIZE; // 一开始用户态没有堆空间

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


/**
 * 一个任务使用一页内存（4K）：
 *
 * +------------+ 4K
 * | intr_frame |
 * +------------+
 * | task_frame |
 * +------------+ <- eip
 * |            |
 * |     ...    |
 * |            |
 * +------------+
 * |    task    | 
 * +------------- 0
 */

// 调用该函数的地方不能有任何局部变量，防止被iframe覆盖
// 调用前栈顶需要准备足够的空间存储iframe
void task_to_user_mode(target_t target)
{
    task_t *task = running_task();

    // 创建用户进程虚拟位图
    task->vmap = kmalloc(sizeof(bitmap_t)); // todo kfree
    void *buf = (void *)alloc_kpage(1);     // todo free_kpage
    // 只能使用8M的虚拟地址
    bitmap_init(task->vmap, buf, PAGE_SIZE, KERNEL_MEMORY_SIZE / PAGE_SIZE);

    // 创建用户进程页表；暂时和内核态的页表一样
    task->pde = (uint32_t)copy_pde();
    set_cr3(task->pde);

    uint32_t addr = (uint32_t)task + PAGE_SIZE;

    addr -= sizeof(intr_frame_t);
    intr_frame_t *iframe = (intr_frame_t *)(addr);
    // 构造一个中断栈帧，用户态中断后会被恢复

    iframe->vector = 0x20;
    iframe->edi = 1;
    iframe->esi = 2;
    iframe->ebp = 3;
    iframe->esp_dummy = 4;
    iframe->ebx = 5;
    iframe->edx = 6;
    iframe->ecx = 7;
    iframe->eax = 8;

    // 修改为用户态的段选择子，不过之前映射的时候用户态的段选择子和内核态映射的是相同的内存。
    iframe->gs = 0;
    iframe->ds = USER_DATA_SELECTOR;
    iframe->es = USER_DATA_SELECTOR;
    iframe->fs = USER_DATA_SELECTOR;
    iframe->ss = USER_DATA_SELECTOR;
    iframe->cs = USER_CODE_SELECTOR;

    iframe->error = OS_MAGIC;

    iframe->eip = (uint32_t)target;
    iframe->eflags = (0 << 12 | 0b10 | 1 << 9);

    // 设置用户栈
    iframe->esp = USER_STACK_TOP;

    asm volatile(
        "movl %0, %%esp\n"
        "jmp interrupt_exit\n" ::"m"(iframe));
}

extern void interrupt_exit();

static void task_build_stack(task_t *task)
{
    uint32_t addr = (uint32_t)task + PAGE_SIZE;
    addr -= sizeof(intr_frame_t);
    intr_frame_t *iframe = (intr_frame_t *)addr;
    // 子进程返回0
    iframe->eax = 0;

    addr -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)addr;

    frame->ebp = 0xaa55aa55;
    frame->ebx = 0xaa55aa55;
    frame->edi = 0xaa55aa55;
    frame->esi = 0xaa55aa55;

    // 构造好后下次任务轮询会跳转到interrupt_exit 然后用户态栈会在iret的时候恢复
    frame->eip = interrupt_exit;

    task->stack = (uint32_t *)frame;
}

pid_t task_fork()
{
    // LOGK("fork is called\n");
    task_t *task = running_task();

    // 当前进程没有阻塞，且正在执行
    assert(task->node.next == NULL && task->node.prev == NULL && task->state == TASK_RUNNING);

    // 拷贝内核栈 和 PCB
    task_t *child = get_free_task();
    pid_t pid = child->pid;
    memcpy(child, task, PAGE_SIZE);

    child->pid = pid;
    child->ppid = task->pid;
    child->ticks = child->priority;
    child->state = TASK_READY;

    // 拷贝用户进程虚拟内存位图
    child->vmap = kmalloc(sizeof(bitmap_t)); // todo kfree
    memcpy(child->vmap, task->vmap, sizeof(bitmap_t));

    // 拷贝虚拟位图缓存
    void *buf = (void *)alloc_kpage(1); // todo free_kpage
    memcpy(buf, task->vmap->bits, PAGE_SIZE);
    child->vmap->bits = buf;

    // 拷贝页目录
    child->pde = (uint32_t)copy_pde();

    // 构造 child 内核栈，设置子进程返回为0
    task_build_stack(child); // ROP

    // 父进程返回子进程 pid
    return child->pid;
}

void task_exit(int status)
{
    task_t *task = running_task();

    // 当前进程没有阻塞，且正在执行
    assert(task->node.next == NULL && task->node.prev == NULL && task->state == TASK_RUNNING);

    task->state = TASK_DIED;
    task->status = status;

    free_pde();

    free_kpage((uint32_t)task->vmap->bits, 1);
    kfree(task->vmap);

    // 将子进程的父进程赋值为自己的父进程
    for (size_t i = 0; i < NR_TASKS; i++)
    {
        task_t *child = task_table[i];
        if (!child)
            continue;
        if (child->ppid != task->pid)
            continue;
        child->ppid = task->ppid;
    }
    LOGK("task 0x%p exit....\n", task);

    // 子进程退出的时候需要唤醒被waitpid阻塞的父进程
    task_t *parent = task_table[task->ppid];
    if (parent->state == TASK_WAITING &&
        (parent->waitpid == -1 || parent->waitpid == task->pid))
    {
        task_unblock(parent);
    }

    schedule();
}


pid_t task_waitpid(pid_t pid, int32_t *status)
{
    task_t *task = running_task();
    task_t *child = NULL;

    while (true)
    {
        bool has_child = false;
        for (size_t i = 2; i < NR_TASKS; i++)
        {
            // 查找子进程
            task_t *ptr = task_table[i];
            if (!ptr)
                continue;

            if (ptr->ppid != task->pid)
                continue;
            if (pid != ptr->pid && pid != -1)
                continue;

            // 如果子进程已经死亡，进入rollback释放子进程
            if (ptr->state == TASK_DIED)
            {
                child = ptr;
                task_table[i] = NULL;
                goto rollback;
            }

            // 如果子进程没有死亡，阻塞调用waitpid的进程，知道子进程死亡后唤醒父进程。
            has_child = true;
        }
        if (has_child)
        {
            task->waitpid = pid;
            task_block(task, NULL, TASK_WAITING);
            continue;
        }
        break;
    }

    // 没找到符合条件的子进程
    return -1;

rollback:
    *status = child->status;
    uint32_t ret = child->pid;
    free_kpage((uint32_t)child, 1);
    return ret;
}

extern void idle_thread();
extern void init_thread();
extern void test_thread();

void task_init()
{
    list_init(&block_list);
    list_init(&sleep_list);

    task_setup();
    idle_task = task_create(idle_thread, "idle", 1, KERNEL_USER);
    task_create(init_thread, "init", 5, NORMAL_USER);
    task_create(test_thread, "test", 5, KERNEL_USER);
}
