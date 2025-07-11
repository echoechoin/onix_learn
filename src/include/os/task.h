#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include <os/types.h>

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

typedef uint32_t target_t();

typedef struct task_t
{
    uint32_t *stack;              // 内核栈
    task_state_t state;           // 任务状态
    uint32_t priority;            // 任务优先级
    uint32_t ticks;               // 剩余时间片
    uint32_t jiffies;             // 上次执行时全局时间片
    uint8_t name[TASK_NAME_LEN];  // 任务名
    uint32_t uid;                 // 用户 id
    uint32_t pde;                 // 页目录物理地址
    struct bitmap_t *vmap;        // 进程虚拟内存位图
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

task_t *running_task();
void schedule();

#endif