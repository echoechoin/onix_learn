#include <os/global.h>
#include <os/string.h>
#include <os/debug.h>
#include <os/printk.h>
#include "os/types.h"

descriptor_t gdt[GDT_SIZE]; // 内核全局描述符表
pointer_t gdt_ptr;          // 内核全局描述符表指针

// 初始化内核全局描述符表
void gdt_init()
{
    DEBUGK("init gdt!!!\n");

    asm volatile("sgdt gdt_ptr");

    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limit + 1);

    gdt_ptr.base = (uint32_t)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;
    asm volatile("lgdt gdt_ptr");
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
