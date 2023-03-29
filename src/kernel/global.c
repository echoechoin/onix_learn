#include "onix/global.h"
#include "onix/string.h"
#include "onix/debug.h"

descriptor_t gdt[GDT_SIZE]; // 内核全局描述符表 我们只用了GDT_SIZE个
pointer_t gdt_ptr;          // 内核全局描述符表指针

// 初始化内核全局描述符表
// 主要是将汇编代码中的全局描述符复制到内核的gdt中
void gdt_init()
{
    // 保存当前的gdt到gdt_ptr
    asm volatile("sgdt gdt_ptr");

    // 复制当前的全局描述符表到gdt    limit = size - 1
    memcpy(&gdt, (void *)gdt_ptr.base, gdt_ptr.limit + 1);

    // 重新设置gdt_ptr
    gdt_ptr.base = (uint32)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    // 加载新的gdt
    asm volatile("lgdt gdt_ptr\n"); 
    DEBUGK("gdt_ptr.base: %#010x, gdt_ptr.limit: %#010x\n", gdt_ptr.base, gdt_ptr.limit);
}
