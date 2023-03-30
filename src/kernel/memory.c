#include "onix/memory.h"

#define ZONE_VALID 1    // ards 可用内存区域
#define ZONE_RESERVED 2 // ards 不可用区域
#define ZONE_UNDEFINED 3

#define IDX(addr) ((uint32)addr >> 12) // 获取 addr 的页索引
#define PAGE(idx) ((uint32)idx << 12) // 获取 idx 的页地址
#define DIDX(addr) (((uint32)addr >> 22) & 0x3ff) // 获取 addr 的页目录索引
#define TIDX(addr) (((uint32)addr >> 12) & 0x3ff) // 获取 addr 的页表索引
#define ASSERT_PAGE(addr) assert((addr & 0xfff) == 0) // 判断 addr 是否是页起始地址

typedef struct ards_t
{
    uint64 base; // 内存基地址
    uint64 size; // 内存长度
    uint32 type; // 类型
} _packed ards_t;

static uint32 memory_base = 0; // 可用内存基地址，应该为0x100000
static uint32 memory_size = 0; // 可用内存大小
static uint32 total_pages = 0; // 所有内存页数
static uint32 free_pages = 0;  // 空闲内存页数

// 内核页目录索引, 在这个物理地址中存储了页目录表
#define KERNEL_PAGE_DIR 0x1000

// 内核页表索引，在这两个物理地址中存储了页表 可以映射8M的物理内存
static uint32 KERNEL_PAGE_TABLE[] = {
    0x2000,
    0x3000,
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define KERNEL_MEMORY_SIZE (ARRAY_SIZE(KERNEL_PAGE_TABLE) * 0x400000)

// 从ards结构体中获取可用内存信息，将最大的一块可用内存信息保存到memory_base和memory_size中
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

    // total_pages 包含了前1M内存和1M以后可用的一块内存
    total_pages = IDX(memory_size) + IDX(MEMORY_BASE);
    free_pages = IDX(memory_size);

    LOGK("Total pages %d\n", total_pages);
    LOGK("Free pages %d\n", free_pages);

    // 如果从1M开始的那块内存大小没有超过内核需要的内存大小，则内核无法运行
    if (memory_size < KERNEL_MEMORY_SIZE)
    {
        panic("System memory is %dM too small, at least %dM needed\n",
              memory_size / MEMORY_BASE, KERNEL_MEMORY_SIZE / MEMORY_BASE);
    }
}

static uint32 start_page = 0;   // 可分配物理内存起始位置
static uint8 *memory_map;       // 物理内存数组
static uint32 memory_map_pages; // 物理内存数组占用的页数

// 设置 cr3 寄存器，参数是页目录的地址
void set_cr3(uint32 pde)
{
    ASSERT_PAGE(pde);
    // 不能直接写 cr3 寄存器，因为这个寄存器是只读的，需要通过 mov 指令来写
    asm volatile("movl %%eax, %%cr3\n" ::"a"(pde));
}

// 得到 cr3 寄存器
uint32 get_cr3()
{
    // 直接将 mov eax, cr3，返回值在 eax 中
    asm volatile("movl %cr3, %eax\n");
}

// 把前1M和1M以后的一块物理内存按照页的方式管理
// 使用一个memory_map数组表示哪些页被使用，哪些页空闲
void memory_map_init()
{
    // 初始化物理内存数组
    memory_map = (uint8 *)memory_base;

    // 计算物理内存数组占用的页数
    // 每页用一个字节表示，所以memory_map_pages需要占用total_pages字节的空间
    // total_pages的空间也需要total_pages/PAGE_SIZE页来保存
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
}

// 分配一页物理内存
static uint32 get_page()
{
    for (size_t i = start_page; i < total_pages; i++)
    {
        // 如果物理内存没有占用
        if (!memory_map[i])
        {
            memory_map[i] = 1;
            free_pages--;
            assert(free_pages >= 0);
            uint32 page = ((uint32)i) << 12;
            LOGK("GET page 0x%p\n", page);
            return page;
        }
    }
    panic("Out of Memory!!!");
}

// 释放一页物理内存
static void put_page(uint32 addr)
{
    ASSERT_PAGE(addr);

    uint32 idx = IDX(addr);

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

// 将 cr0 寄存器最高位 PE 置为 1，启用分页
static inline void enable_page()
{
    // 0b1000_0000_0000_0000_0000_0000_0000_0000
    // 0x80000000
    asm volatile(
        "movl %cr0, %eax\n"
        "orl $0x80000000, %eax\n"
        "movl %eax, %cr0\n");
}

// 初始化页表项
static void entry_init(page_entry_t *entry, uint32 index)
{
    *(uint32 *)entry = 0;
    entry->present = 1; // 在内存中
    entry->write = 1; // 可读可写
    entry->user = 1; // 所有用户都可以访问
    entry->index = index;
}

// 初始化内存映射，前8M的内存映射后虚拟地址和物理地址相同，保证内核可以访问到之前初始化好的内存
// 每个页表的最后一个页表项指向页目录自身，用于实现页目录的循环
void mapping_init()
{
    // 页目录存放地址
    page_entry_t *pde = (page_entry_t *)KERNEL_PAGE_DIR;
    memset(pde, 0, PAGE_SIZE);

    // 初始化两个页表
    for (idx_t didx = 0; didx < ARRAY_SIZE(KERNEL_PAGE_TABLE); didx++)
    {
        // 页表存放地址
        page_entry_t *pte = (page_entry_t *)KERNEL_PAGE_TABLE[didx];
        memset(pte, 0, PAGE_SIZE);

        // 初始化页表起始地址到页目录项
        page_entry_t *dentry = &pde[didx];
        entry_init(dentry, IDX((uint32)pte));

        // 初始化页表项
        for (idx_t tidx = 0; tidx < 1024; tidx++)
        {
            // 第0个页表不初始化，表示0地址无法访问
            if (didx == 0 && tidx == 0)
                continue;

            idx_t index = didx * 1024 + tidx;
            
            page_entry_t *tentry = &pte[tidx];
            entry_init(tentry, index);
            memory_map[index] = 1; // 表示物理地址所在的页已经被占用
        }
    }

    // 将最后一个页目录项指向页目录自身
    page_entry_t *tentry = &pde[1023];
    entry_init(tentry, IDX(KERNEL_PAGE_DIR));

    // 设置 cr3 寄存器： 设置页目录的地址
    set_cr3((uint32)pde);

    BMB;

    // 分页有效 置位cr0的最高位
    enable_page();

    BMB;
}

// 刷新虚拟地址 vaddr 的 块表 TLB
static void flush_tlb(uint32 vaddr)
{
    asm volatile("invlpg (%0)" ::"r"(vaddr)
                 : "memory");
}

// 获取页目录pde[1024]的虚拟地址
// 这里的页目录是可以修改的
static page_entry_t *get_pde()
{
    // 页目录的最后一项指向的是页目录自己
    // 所以第1024个页表就是页目录，因此第1024个页表的最后一项也是指向页目录自己
    return (page_entry_t *)(0xfffff000);
}

// 根据虚拟地址获取页表pte[1024]的虚拟地址
// 这里的页表是可以修改的

// 这个函数就可以修改虚拟地址映射的物理地址了！！！
static page_entry_t *get_pte(uint32 vaddr)
{
    // 1. 0xffc00000表示的是最后一个页目录项
    // 2. 最后一个页目录项指向的就是页目录本身
    // 3. 从页目录中通过页目录项的索引获取页表的虚拟地址
    return (page_entry_t *)(0xffc00000 | (DIDX(vaddr) << 12));
}