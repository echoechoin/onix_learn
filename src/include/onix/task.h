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

// 中断帧
typedef struct intr_frame_t
{
    // 系统调用：vector是返回值
    // 中断/异常：vector是中断号
    uint32 vector; 

    uint32 edi;
    uint32 esi;
    uint32 ebp;
    // 虽然 pushad 把 esp 也压入，但 esp 是不断变化的，所以会被 popad 忽略
    uint32 esp_dummy;

    uint32 ebx;
    uint32 edx;
    uint32 ecx;
    uint32 eax;

    uint32 gs;
    uint32 fs;
    uint32 es;
    uint32 ds;

    uint32 vector0; // 中断 / 异常向量号

    uint32 error; // 如果是异常，cpu会额外压入一个错误码

    uint32 eip;
    uint32 cs;
    uint32 eflags;
    uint32 esp3;
    uint32 ss3;
} intr_frame_t;

void task_init();
task_t *running_task();
void schedule();
void task_yield();
void task_block(task_t *task, list_t *blist, task_state_t state);
void task_unblock(task_t *task);
void task_sleep(uint32 ms);
void task_wakeup();
void task_to_user_mode(target_t target);

#endif