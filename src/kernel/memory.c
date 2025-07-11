#include <os/memory.h>
#include <os/types.h>
#include <os/debug.h>
#include <os/assert.h>
#include <os/os.h>
#include "os/string.h"
#include "os/stdlib.h"
#include "os/printk.h"
#include "os/bitmap.h"

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

#define IDX(addr) ((uint32_t)addr >> 12)            // 获取 paddr 的页索引
#define DIDX(addr) (((uint32_t)addr >> 22) & 0x3ff) // 获取 vaddr 的页目录列表索引: addr的高10位
#define TIDX(addr) (((uint32_t)addr >> 12) & 0x3ff) // 获取 vaddr 的页表索引: addr的中间10位

#define PAGE(idx) ((uint32_t)idx << 12)             // 获取页索引 idx 对应的页开始的位置: 1 -> 0x1000
#define ASSERT_PAGE(addr) assert((addr & 0xfff) == 0)

// 指定内核页目录索引 (pde[1024] 列表起始地址)，之后会存放到 cr3 寄存器里
#define KERNEL_PAGE_DIR 0x1000 // 页目录的起始地址，可以存放 1024 个页表

// 内核页表索引, 表示我们使用的页表
static uint32_t KERNEL_PAGE_TABLE[] = {
    0x2000, // 每个页表的起始地址，可以存放 1024 个页表 pde[0].pte[1024]
    0x3000, // pde[1].pte[1024]
};

// 2 个页表 * 1024 个页每个页表 * 4K每页  = 8M
#define KERNEL_MEMORY_SIZE (0x100000 * sizeof(KERNEL_PAGE_TABLE))

#define KERNEL_MAP_BITS 0x4000

bitmap_t kernel_map; // 内核虚拟内存占用状态

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


    if (memory_size < KERNEL_MEMORY_SIZE)
    {
        panic("System memory is %dM too small, at least %dM needed\n",
              memory_size / MEMORY_BASE, KERNEL_MEMORY_SIZE / MEMORY_BASE);
    }
}

static uint32_t start_page = 0;   // 可分配物理内存起始位置
static uint8_t *memory_map;       // 物理内存数组
static uint32_t memory_map_pages; // 物理内存数组占用的页数

void memory_map_init()
{
    // 初始化物理内存数组，存放物理页占用情况
    memory_map = (uint8_t *)memory_base;

    // 计算物理内存数组占用的页
    memory_map_pages = div_round_up(total_pages, PAGE_SIZE);
    LOGK("Memory map page count %d\n", memory_map_pages);

    free_pages -= memory_map_pages;

    // 清空物理内存数组
    memset((void *)memory_map, 0, memory_map_pages * PAGE_SIZE);

    // 前 1M 的内存位置 以及 物理内存数组已占用的页，已被占用
    start_page = IDX(MEMORY_BASE) + memory_map_pages;
    for (size_t i = 0; i < start_page; i++)
    {
        memory_map[i] = 1;
    }

    LOGK("Total pages %d free pages %d\n", total_pages, free_pages);

    // 初始化内核虚拟内存位图，需要 8 位对齐
    uint32_t length = (IDX(KERNEL_MEMORY_SIZE) - IDX(MEMORY_BASE)) / 8;
    bitmap_init(&kernel_map, (char *)KERNEL_MAP_BITS, length, IDX(MEMORY_BASE));
    bitmap_scan(&kernel_map, memory_map_pages); // 将已经使用的内存页置1
}

// 通过memory_map数组判断某个页是否被占用，分配一页物理内存
static uint32_t get_page()
{
    for (size_t i = start_page; i < total_pages; i++)
    {
        // 如果物理内存没有占用
        if (!memory_map[i])
        {
            memory_map[i] = 1;
            free_pages--;
            assert(free_pages >= 0);
            uint32_t page = ((uint32_t)i) << 12;
            LOGK("GET page 0x%p\n", page);
            return page;
        }
    }
    panic("Out of Memory!!!");
}

// 释放一页物理内存
static void put_page(uint32_t addr)
{
    ASSERT_PAGE(addr);

    uint32_t idx = IDX(addr);

    // idx 大于 1M 并且 小于 总页面数
    assert(idx >= start_page && idx < total_pages);

    // 保证只有一个引用
    assert(memory_map[idx] >= 1);

    // 物理引用减一
    memory_map[idx]--;

    // 若为 0，则空闲页加一
    if (!memory_map[idx])
    {
        free_pages++;
    }

    assert(free_pages > 0 && free_pages < total_pages);
    LOGK("PUT page 0x%p\n", addr);
}

// 得到 cr3 寄存器
uint32_t inline get_cr3()
{
    // 直接将 mov eax, cr3，返回值在 eax 中
    asm volatile("movl %cr3, %eax\n");
}

// 设置 cr3 寄存器，参数是页目录的地址
void set_cr3(uint32_t pde)
{
    ASSERT_PAGE(pde);
    asm volatile("movl %%eax, %%cr3\n" ::"a"(pde));
}

// 将 cr0 寄存器最高位 PG 置为 1，启用分页
static _inline void enable_page()
{
    // 0b1000_0000_0000_0000_0000_0000_0000_0000
    // 0x80000000
    asm volatile(
        "movl %cr0, %eax\n"
        "orl $0x80000000, %eax\n"
        "movl %eax, %cr0\n");
}

// 初始化页表项
static void entry_init(page_entry_t *entry, uint32_t index)
{
    *(uint32_t *)entry = 0;
    entry->present = 1;
    entry->write = 1;
    entry->user = 1;
    entry->index = index;
}

// 初始化内存映射
void mapping_init()
{
    page_entry_t *pde = (page_entry_t *)KERNEL_PAGE_DIR;
    memset(pde, 0, PAGE_SIZE);

    idx_t index = 0;

    for (idx_t didx = 0; didx < (sizeof(KERNEL_PAGE_TABLE) / 4); didx++)
    {
        page_entry_t *pte = (page_entry_t *)KERNEL_PAGE_TABLE[didx];
        memset(pte, 0, PAGE_SIZE);

        page_entry_t *dentry = &pde[didx];
        entry_init(dentry, IDX((uint32_t)pte)); // pde->index 存放 pte 的物理地址 >> 12

        for (idx_t tidx = 0; tidx < 1024; tidx++, index++)
        {
            // 第 0 页不映射，为造成空指针访问，缺页异常，便于排错
            if (index == 0)
                continue;

            page_entry_t *tentry = &pte[tidx];
            entry_init(tentry, index); // pte->index 存放 页的物理基地址 >> 12
            memory_map[index] = 1; // 设置物理内存数组，该页被占用
        }
    }

    // 将最后一个页表指向页目录自己，方便修改
    page_entry_t *entry = &pde[1023];
    entry_init(entry, IDX(KERNEL_PAGE_DIR));

    // 设置 cr3 寄存器
    set_cr3((uint32_t)pde);

    // 分页有效
    enable_page();
}

static page_entry_t *get_pde()
{
    return (page_entry_t *)(0xfffff000);
}

static page_entry_t *get_pte(uint32_t vaddr)
{
    return (page_entry_t *)(0xffc00000 | (DIDX(vaddr) << 12));
}

// 刷新虚拟地址 vaddr 的 块表 TLB
static void flush_tlb(uint32_t vaddr)
{
    asm volatile("invlpg (%0)" ::"r"(vaddr)
                 : "memory");
}


// 从位图中扫描 count 个连续的页
static uint32_t scan_page(bitmap_t *map, uint32_t count)
{
    assert(count > 0);
    int32_t index = bitmap_scan(map, count);

    if (index == EOF)
    {
        panic("Scan page fail!!!");
    }

    uint32_t addr = PAGE(index);
    LOGK("Scan page 0x%p count %d\n", addr, count);
    return addr;
}

// 与 scan_page 相对，重置相应的页
static void reset_page(bitmap_t *map, uint32_t addr, uint32_t count)
{
    ASSERT_PAGE(addr);
    assert(count > 0);
    uint32_t index = IDX(addr);

    for (size_t i = 0; i < count; i++)
    {
        assert(bitmap_test(map, index + i));
        bitmap_set(map, index + i, 0);
    }
}

// 分配 count 个连续的内核页
uint32_t alloc_kpage(uint32_t count)
{
    assert(count > 0);
    uint32_t vaddr = scan_page(&kernel_map, count);
    LOGK("ALLOC kernel pages 0x%p count %d\n", vaddr, count);
    return vaddr;
}

// 释放 count 个连续的内核页
void free_kpage(uint32_t vaddr, uint32_t count)
{
    ASSERT_PAGE(vaddr);
    assert(count > 0);
    reset_page(&kernel_map, vaddr, count);
    LOGK("FREE  kernel pages 0x%p count %d\n", vaddr, count);
}

// void memory_test()
// {
//     // 将 20 M 0x1400000 内存映射到 64M 0x4000000 的位置
//     // 我们还需要一个页表，0x900000

//     uint32_t vaddr = 0x4000000; // 线性地址几乎可以是任意的 // pte 为第16个页目录项的第0个页表项
//     uint32_t paddr = 0x1ff000;  // 物理地址必须要确定存在   // 将0x1ff 放到pte里
//     uint32_t table = 0x900000;  // 页表也必须是物理地址     // 将0x900  放到pde里

//     page_entry_t *pde = get_pde();

//     page_entry_t *dentry = &pde[DIDX(vaddr)];            // 获取第16个页目录项
//     entry_init(dentry, IDX(table));         // 将0x900 放到pde里

//     page_entry_t *pte = get_pte(vaddr);                  // 获取页表基地址
//     page_entry_t *tentry = &pte[TIDX(vaddr)];            // 获取第0个页表项pte

//     entry_init(tentry, IDX(paddr));         // 将0x1ff 放到pte里, 此时访问 0x4000000 就是访问 物理地址 0x1ff000

//     char *ptr1 = (char *)(vaddr);
//     char *ptr2 = (char *)(paddr);
//     strcpy(ptr1, "hello world!");

//     printk("ptr1 = %p (%s)\n", ptr1, ptr1);
//     printk("ptr2 = %p (%s)\n", ptr2, ptr2);  // 可以看到修改了ptr1, ptr2也会被修改，因为ptr1和ptr2都是映射到同一个物理地址0x1ff000

//     // entry_init(tentry, IDX(0x1500000));
//     // flush_tlb(vaddr);

// }
