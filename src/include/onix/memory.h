#ifndef ONIX_MEMORY_H
#define ONIX_MEMORY_H
#include "onix/onix.h"
#include "onix/types.h"
#include "onix/debug.h"
#include "onix/types.h"
#include "onix/assert.h"
#include "onix/stdlib.h"
#include "onix/string.h"
#include <onix/bitmap.h>

#define PAGE_SIZE 0x1000     // 一页的大小 4K
#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置

typedef struct page_entry_t
{
    uint8 present : 1;  // 在内存中
    uint8 write : 1;    // 0 只读 1 可读可写
    uint8 user : 1;     // 1 所有人 0 超级用户 DPL < 3
    uint8 pwt : 1;      // page write through 1 直写模式，0 回写模式
    // 直写: 立即将修改的缓存块写回内存, 缺点是每次写都会引起总线流量
    // 回写: 当被修改的块被换出时再写回内存, 缺点是增加复杂性, 需要维护一个修改位

    // 在回写模式下，当缓存块被修改时，更改不会立即写回内存。相反，缓存控制器会将修改过的缓存块标记为“脏”（dirty），并在稍后的某个时间将其写回内存。这个时间点通常是当缓存控制器需要为新的数据腾出空间时，它会选择一个脏块并将其写回内存，然后再使用该块来存储新的数据。

    uint8 pcd : 1;      // page cache disable 禁止该页缓冲到cache中
    uint8 accessed : 1; // 被访问过，用于统计使用频率
    uint8 dirty : 1;    // 脏页，表示该页缓冲被写过，用于置换到磁盘的时候判断是否需要更新磁盘中的内容
    uint8 pat : 1;      // page attribute table 页大小 4K/4M，80386只支持4K页
    uint8 global : 1;   // 全局，所有进程都用到了，该页不刷新缓冲
    uint8 ignored : 3;  // 该安排的都安排了，送给操作系统吧
    uint32 index : 20;  // 页索引
} _packed page_entry_t;

void set_cr3(uint32 pde);
uint32 get_cr3();

void memory_init(uint32 magic, uint32 addr);
void memory_map_init();
void mapping_init();

uint32 alloc_kpage(uint32 count);
void free_kpage(uint32 vaddr, uint32 count);
#endif
