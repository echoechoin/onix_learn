#ifndef OS_MEMORY_H
#define OS_MEMORY_H

#include "os/types.h"

#define PAGE_SIZE 0x1000     // 一页的大小 4K
#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置

typedef struct page_entry_t
{
    uint8_t present : 1;  // 在内存中
    uint8_t write : 1;    // 0 只读 1 可读可写
    uint8_t user : 1;     // 1 所有人 0 超级用户 DPL < 3
    uint8_t pwt : 1;      // page write through 1 直写模式，0 回写模式
    uint8_t pcd : 1;      // page cache disable 禁止该页缓冲
    uint8_t accessed : 1; // 被访问过，用于统计使用频率
    uint8_t dirty : 1;    // 脏页，表示该页缓冲被写过
    uint8_t pat : 1;      // page attribute table 页大小 4K/4M
    uint8_t global : 1;   // 全局，所有进程都用到了，该页不刷新缓冲
    uint8_t ignored : 3;  // 该安排的都安排了，送给操作系统吧
    uint32_t index : 20;  // 页索引
} _packed page_entry_t;

uint32_t get_cr3();
void set_cr3(uint32_t pde);

void memory_init(uint32_t magic, uint32_t addr);

#endif