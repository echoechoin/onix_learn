#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include <os/types.h>

typedef uint32_t target_t();

typedef struct task_t
{
    uint32_t *stack; // 内核栈
} task_t;

typedef struct task_frame_t
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebx;
    uint32_t ebp;
    void (*eip)(void);
} task_frame_t;

void task_init();

#endif