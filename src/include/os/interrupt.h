#ifndef OS_INTERRUPT_H
#define OS_INTERRUPT_H

#include <os/types.h>
#include <os/printk.h>

#define IDT_SIZE 256

typedef void (*handler_t)(int); // 中断处理函数

typedef struct gate_t
{
    uint16_t offset0;    // 段内偏移 0 ~ 15 位
    uint16_t selector;   // 代码段选择子
    uint8_t reserved;    // 保留不用
    uint8_t type : 4;    // 任务门/中断门/陷阱门
    uint8_t segment : 1; // segment = 0 表示系统段
    uint8_t DPL : 2;     // 使用 int 指令访问的最低权限
    uint8_t present : 1; // 是否有效
    uint16_t offset1;    // 段内偏移 16 ~ 31 位
} _packed gate_t;

void interrupt_init();

#endif