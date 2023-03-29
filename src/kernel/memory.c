#include "onix/memory.h"

#define ZONE_VALID 1    // ards 可用内存区域
#define ZONE_RESERVED 2 // ards 不可用区域
#define ZONE_UNDEFINED 3

typedef struct ards_t
{
    uint64 base; // 内存基地址
    uint64 size; // 内存长度
    uint32 type; // 类型
} _packed ards_t;

uint32 memory_base = 0; // 可用内存基地址，应该等于 1M
uint32 memory_size = 0; // 可用内存大小
uint32 total_pages = 0; // 所有内存页数
uint32 free_pages = 0;  // 空闲内存页数

void memory_init(uint32 magic, uint32 addr)
{
    uint32 count = *((uint32*)(addr)); // ards 数量
    ards_t *ptr = (ards_t *)(addr  + 4); // ards 结构体数组的首地址

    if (magic != ONIX_MAGIC)
    {
        panic("memory_init: magic number is wrong!");
    }

    for (size_t i = 0; i < count; i++, ptr++)
    {
        LOGK("ards: base: %x, size: %x, type: %d\n", (uint32)ptr->base, (uint32)ptr->size, (uint32)ptr->type);

        // 找到最大的可用内存区域
        if (ptr->type == ZONE_VALID && ptr->size > memory_size)
        {
            memory_base = ptr->base;
            memory_size = ptr->size;
        }
    }

    LOGK("ARDS count %d\n", count);
    LOGK("Memory base 0x%p\n", (uint32)memory_base);
    LOGK("Memory size 0x%p\n", (uint32)memory_size);

    assert(memory_base == MEMORY_BASE); // 内存开始的位置为 1M
    assert((memory_size & 0xfff) == 0); // 要求按页对齐

    #define IDX(addr) ((uint32)addr >> 12) // 获取 addr 的页索引
    total_pages = IDX(memory_size) + IDX(MEMORY_BASE);
    free_pages = IDX(memory_size);

    LOGK("Total pages %d\n", total_pages);
    LOGK("Free pages %d\n", free_pages);
}