#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include "onix/types.h"
#include "onix/list.h"

#define KERNEL_USER 0
#define NORMAL_USER 1
#define TASK_NAME_LEN 16

// 进程主函数
typedef void (*target_t)();

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

// 类似linux的PCB
typedef struct task_t
{
    uint32 *stack;              // 内核栈
    list_node_t node;                // 任务阻塞节点
    task_state_t state;         // 任务状态
    uint32 priority;            // 任务优先级
    uint32 ticks;               // 剩余时间片
    uint32 jiffies;             // 上次执行时全局时间片
    uint8 name[TASK_NAME_LEN];  // 任务名
    uint32 uid;                 // 用户 id
    uint32 pde;                 // 页目录物理地址
    struct bitmap_t *vmap;      // 进程虚拟内存位图
    uint32 magic;               // 内核魔数，用于检测栈溢出
} task_t;

typedef struct task_frame_t
{
    uint32 edi;        // 手动保存的寄存器
    uint32 esi;        // 手动保存的寄存器
    uint32 ebx;        // 手动保存的寄存器
    uint32 ebp;        // 保存栈帧时
    void (*eip)(void); // 调用call的时候会把eip压入栈中
} task_frame_t;


void task_init();
task_t *running_task();
void schedule();
void task_yield();
void task_block(task_t *task, list_t *blist, task_state_t state);
void task_unblock(task_t *task);
void task_sleep(uint32 ms);
void task_wakeup();

#endif