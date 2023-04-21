#include "onix/global.h"
#include "onix/string.h"
#include "onix/debug.h"

descriptor_t gdt[GDT_SIZE]; // 内核全局描述符表 我们只用了GDT_SIZE个
pointer_t gdt_ptr;          // 内核全局描述符表指针
tss_t tss;                  // 任务状态段

void descriptor_init(descriptor_t *desc, uint32 base, uint32 limit)
{
    desc->base_low = base & 0xffffff;
    desc->base_high = (base >> 24) & 0xff;
    desc->limit_low = limit & 0xffff;
    desc->limit_high = (limit >> 16) & 0xf;
}

// 初始化内核全局描述符表
void gdt_init()
{
    DEBUGK("init gdt!!!\n");

    memset(gdt, 0, sizeof(gdt));
    // 内核代码段
    descriptor_t *desc;
    desc = gdt + KERNEL_CODE_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 代码段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 0;         // **内核特权级**
    desc->type = 0b1010;   // 代码 / 非依从 / 可读 / 没有被访问过

    // 内核数据段
    desc = gdt + KERNEL_DATA_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 数据段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 0;         // **内核特权级**
    desc->type = 0b0010;   // 数据 / 向上增长 / 可写 / 没有被访问过

    desc = gdt + USER_CODE_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 代码段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 3;         // **用户特权级**
    desc->type = 0b1010;   // 代码 / 非依从 / 可读 / 没有被访问过

    desc = gdt + USER_DATA_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 数据段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 3;         // **用户特权级**
    desc->type = 0b0010;   // 数据 / 向上增长 / 可写 / 没有被访问过

    gdt_ptr.base = (uint32)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;
}

void tss_init()
{
    memset(&tss, 0, sizeof(tss));

    tss.ss0 = KERNEL_DATA_SELECTOR; // 内核数据段
    tss.iobase = sizeof(tss); // 没有用到，直接指向tss的结束位置（ I/O Map Base Address Field. Contains a 16-bit offset from the base of the TSS to the I/O Permission Bit Map.）

    descriptor_t *desc = gdt + KERNEL_TSS_IDX;
    descriptor_init(desc, (uint32)&tss, sizeof(tss) - 1);
    desc->segment = 0;     // 系统段
    desc->granularity = 0; // 字节 TODO: 为什么是0
    desc->big = 0;         // 固定为 0
    desc->long_mode = 0;   // 固定为 0
    desc->present = 1;     // 在内存中
    desc->DPL = 0;         // 用于任务门或调用门
    // segment == 0的时候
    // | type | desc             |
    // | ---- | ---------------- |
    // | 0000 | 保留             |
    // | 0001 | 16 位 TSS (可用) |
    // | 0010 | LDT              |
    // | 0011 | 16 位 TSS (忙)   |
    // | 0100 | 16 位 调用门     |
    // | 0101 | 任务门           |
    // | 0110 | 16 位 中断门     |
    // | 0111 | 16 位 陷阱门     |
    // | 1000 | 保留             |
    // | 1001 | 32 位 TSS (可用) |
    // | 1010 | 保留             |
    // | 1011 | 32 位 TSS (忙)   |
    // | 1100 | 32 位 调用门     |
    // | 1101 | 保留             |
    // | 1110 | 32 位 中断门     |
    // | 1111 | 32 位 陷阱门     |
    desc->type = 0b1001;   // 32 位可用 tss

    asm volatile(
        "ltr %%ax\n" ::"a"(KERNEL_TSS_SELECTOR));
}
