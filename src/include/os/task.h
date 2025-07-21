#ifndef OS_TASK_H
#define OS_TASK_H

#include <os/types.h>
#include <os/list.h>

#define KERNEL_USER 0
#define NORMAL_USER 1

#define TASK_NAME_LEN 16

typedef enum task_state_t
{
    TASK_INIT,     // 初始化
    TASK_RUNNING,  // 执行
    TASK_READY,    // 就绪
    TASK_BLOCKED,  // 阻塞
    TASK_SLEEPING, // 睡眠
    TASK_WAITING,  // 等待
    TASK_DIED,     // 死亡
} task_state_t;

typedef void(*target_t)();

typedef struct task_t
{
    uint32_t *stack;              // 切换任务的时候，保存当前任务的内核栈
    list_node_t node;             // 任务阻塞节点
    task_state_t state;           // 任务状态
    uint32_t priority;            // 任务优先级
    uint32_t ticks;               // 剩余时间片
    uint32_t jiffies;             // 上次执行时全局时间片
    uint8_t name[TASK_NAME_LEN];  // 任务名
    uint32_t uid;                 // 用户 id
    pid_t pid;                    // 任务 id
    pid_t ppid;                   // 父任务 id
    uint32_t pde;                 // 页目录物理地址
    struct bitmap_t *vmap;        // 进程虚拟内存位图
    uint32_t brk;                 // 进程堆内存最高地址
    int status;                   // 进程退出值
    pid_t waitpid;                // 进程等待的 pid
    uint32_t magic;               // 内核魔数，用于检测栈溢出
} task_t;

typedef struct task_frame_t
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    void (*eip)(void);
} task_frame_t;

/**
 * 在 x86 架构中，当发生中断或异常（如硬件中断、软件中断、页面错误等）时，处理器会自动将部分寄存器状态压入堆栈，形成一个中断栈帧。
 * 操作系统可能还会额外保存其他寄存器，以便在中断处理完成后恢复上下文。这个 intr_frame_t 结构体就是对这种栈帧的精确描述，用于：
 * 
 * 保存用户态或内核态的上下文（寄存器状态）。
 * 在中断处理程序中操作这些值。
 * 在中断返回（iret）时恢复上下文。
 **/

typedef struct intr_frame_t
{
    /* 5. 保存的中断向量号 */
    uint32_t vector;

    /* 4. 通用寄存器 对应pusha */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    // 虽然 pushad 把 esp 也压入，但 esp 是不断变化的，所以会被 popad 忽略
    uint32_t esp_dummy;

    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    /* 3. 段寄存器 对应handler.asm 中的 push ds es fs gs */
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t vector0; // 压入中断向量号

    /* 2. 错误码，没有错误码的中断，我们会在handler.asm中压入一个固定值。*/
    uint32_t error;

    /* 1. CPU自动压入这些值 */
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} intr_frame_t;

task_t *running_task();
void schedule();
void task_yield();
void task_exit(int status);
pid_t task_fork();
pid_t task_waitpid(pid_t pid, int32_t *status);

void task_block(task_t *task, list_t *blist, task_state_t state);
void task_unblock(task_t *task);

void task_sleep(uint32_t ms);
void task_wakeup();

void task_to_user_mode(target_t target);

pid_t sys_getpid();
pid_t sys_getppid();
#endif