#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include <onix/types.h>

// 进程主函数
typedef uint32 target_t();

// 类似linux的PCB
typedef struct task_t
{
    uint32 *stack; // 内核栈：存储当前任务的esp
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

#endif