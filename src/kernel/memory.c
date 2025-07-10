#include <os/memory.h>
#include <os/types.h>
#include <os/debug.h>
#include <os/assert.h>
#include <os/os.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define for_each_ards_type \
    _(RESERVED,      "-") \
    _(ZONE_VALID,    "Avaiable") \
    _(ZONE_RESERVED, "Reserved") \

typedef enum {
#define _(t, d) t,
    for_each_ards_type
#undef _
} ards_type_t;

static const char *ards_type_s[] = {
#define _(t, d) d,
    for_each_ards_type
#undef _
};

#define IDX(addr) ((uint32_t)addr >> 12) // 获取 addr 的页索引

typedef struct ards_t
{
    uint64_t base; // 内存基地址
    uint64_t size; // 内存长度
    union {
        uint32_t _;
        ards_type_t type; // 类型
    };
} _packed ards_t;

uint32_t memory_base = 0; // 可用内存基地址，应该等于 1M
uint32_t memory_size = 0; // 可用内存大小
uint32_t total_pages = 0; // 所有内存页数
uint32_t free_pages = 0;  // 空闲内存页数

#define used_pages (total_pages - free_pages) // 已用页数

void memory_init(uint32_t magic, uint32_t addr)
{
    uint32_t count;
    ards_t *ptr;

    // 如果是 onix loader 进入的内核
    if (magic != OS_MAGIC)
        panic("Memory init magic unknown 0x%p\n", magic);

    count = *(uint32_t *)addr;
    ptr = (ards_t *)(addr + 4);

    for (size_t i = 0; i < count; i++, ptr++) {
        LOGK("Memory base 0x%08x size 0x%08x type %s\n",
                (uint32_t)ptr->base, (uint32_t)ptr->size,
                ptr->type <= ZONE_RESERVED ? ards_type_s[ptr->type]: ards_type_s[0]);
        if (ptr->type == ZONE_VALID && ptr->size > memory_size) {
            memory_base = (uint32_t)ptr->base;
            memory_size = (uint32_t)ptr->size;
        }
    }

    LOGK("ARDS count %d\n", count);
    LOGK("Memory base 0x%p\n", (uint32_t)memory_base);
    LOGK("Memory size 0x%p\n", (uint32_t)memory_size);

    assert(memory_base == MEMORY_BASE); // 内存开始的位置为 1M
    assert((memory_size & 0xfff) == 0); // 要求按页对齐

    total_pages = IDX(memory_size) + IDX(MEMORY_BASE);
    free_pages = IDX(memory_size);

    LOGK("Total pages %d\n", total_pages);
    LOGK("Free pages %d\n", free_pages);
}
