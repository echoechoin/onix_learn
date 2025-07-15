#include <os/global.h>
#include <os/string.h>
#include <os/debug.h>
#include <os/printk.h>
#include "os/types.h"

descriptor_t gdt[GDT_SIZE]; // 内核全局描述符表
pointer_t gdt_ptr;          // 内核全局描述符表指针
tss_t tss;                  // 任务状态段

void descriptor_init(descriptor_t *desc, uint32_t base, uint32_t limit)
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

    descriptor_t *desc;
    desc = gdt + KERNEL_CODE_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 代码段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 0;         // 内核特权级
    desc->type = 0b1010;   // 代码 / 非依从 / 可读 / 没有被访问过

    desc = gdt + KERNEL_DATA_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 数据段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 0;         // 内核特权级
    desc->type = 0b0010;   // 数据 / 向上增长 / 可写 / 没有被访问过

    // 用户态暂时和内核态使用一样的内存
    desc = gdt + USER_CODE_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 代码段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 3;         // 用户特权级
    desc->type = 0b1010;   // 代码 / 非依从 / 可读 / 没有被访问过

    desc = gdt + USER_DATA_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // 数据段
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 位
    desc->long_mode = 0;   // 不是 64 位
    desc->present = 1;     // 在内存中
    desc->DPL = 3;         // 用户特权级
    desc->type = 0b0010;   // 数据 / 向上增长 / 可写 / 没有被访问过

    gdt_ptr.base = (uint32_t)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;
    asm volatile("lgdt gdt_ptr");
}

void tss_init()
{
    memset(&tss, 0, sizeof(tss));

    tss.ss0 = KERNEL_DATA_SELECTOR;
    tss.iobase = sizeof(tss);

    descriptor_t *desc = gdt + KERNEL_TSS_IDX;
    descriptor_init(desc, (uint32_t)&tss, sizeof(tss) - 1);
    desc->segment = 0;     // 系统段
    desc->granularity = 0; // 字节
    desc->big = 0;         // 固定为 0
    desc->long_mode = 0;   // 固定为 0
    desc->present = 1;     // 在内存中
    desc->DPL = 0;         // 用于任务门或调用门
    desc->type = 0b1001;   // 32 位可用 tss

    asm volatile(
        "ltr %%ax\n" ::"a"(KERNEL_TSS_SELECTOR));
}

void show_gdt_list()
{
    DEBUGK("gdt summary:\n");
    DEBUGK("\t base: 0x%x\n", gdt_ptr.base);
    DEBUGK("\t limit: 0x%x\n\n", gdt_ptr.limit);

    for (int i = 0; i < 3 /*GDT_SECTOR_NUM(&gdt_ptr)*/; i++) {
        DEBUGK("gdt[%d]:\n", i);
        if (i == 0) {
            DEBUGK("default placeholder\n");
            continue;
        }
        
        uint32_t limit; // 段界限
        uint32_t base; // 段基址

        limit = gdt[i].limit_low & 0xFFFF;
        limit |= gdt[i].limit_high << 16;
        DEBUGK("\t limit: 0x%x\n", limit << 12 | 0xfff);

        base = gdt[i].base_low & 0xFFFF;
        base |= gdt[i].base_high << 16;
        DEBUGK("\t base: 0x%x\n", base);
    }

}
