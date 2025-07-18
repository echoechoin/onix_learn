#ifndef OS_MEMORY_H
#define OS_MEMORY_H

#include "os/types.h"

// 内核占用的内存大小 8M (整个操作系统占用的内存大小)
#define KERNEL_MEMORY_SIZE 0x800000

// 用户栈顶地址 128M
#define USER_STACK_TOP 0x8000000

// 用户栈最大 2M
#define USER_STACK_SIZE 0x200000

// 用户栈底地址 128M - 2M (虚拟地址)
#define USER_STACK_BOTTOM (USER_STACK_TOP - USER_STACK_SIZE)

#define PAGE_SIZE 0x1000     // 一页的大小 4K
#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置

// 指定内核页目录索引 (pde[1024] 列表起始地址)，之后会存放到 cr3 寄存器里
#define KERNEL_PAGE_DIR 0x1000 // 页目录的起始地址，可以存放 1024 个页表

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

// 得到 cr2 寄存器; 存储了造成缺页异常的虚拟地址
uint32_t get_cr2();

uint32_t get_cr3();
void set_cr3(uint32_t pde);

uint32_t alloc_kpage(uint32_t count);
void free_kpage(uint32_t vaddr, uint32_t count);

void memory_init(uint32_t magic, uint32_t addr);

page_entry_t *copy_pde();
void free_pde();

// 系统调用 brk
int32_t sys_brk(void *addr);
#endif