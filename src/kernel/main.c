#include "onix/onix.h"
#include "onix/types.h"
#include "onix/io.h"
#include "onix/console.h"
#include "onix/stdarg.h"
#include "onix/printk.h"
#include "onix/assert.h"
#include "onix/debug.h"
#include "onix/global.h"
#include "onix/task.h"
#include "onix/interrupt.h"

// 栈内存对齐测试
void stack_align(char arg1, int arg2, char arg3, int arg4)
{
    // -exec x/40x 0xffb8
    // 0xffb8: `0x00012103``0x00000001` 0x000003ff	0x0000114b // 为什么arg1 和 arg3的地址是在这里？
    // 0xffc8:	0x0000fff8	0x00010088 `0x00000001``0x00000002`
    // 0xffd8: `0x00000003``0x00000004`	0x00000000	0x00000000
    // 0xffe8:	0xffff0000	0x00000001	0x80000000	0x11150001
    // 0xfff8:	0x00000000	0x0001000f	0x800005c6	0x664b000b
    DEBUGK("arg1: %#010x, arg2: %#010x, arg3: %#010x, arg4: %#010x\n",
        arg1, arg2, arg3, arg4);
}

void kernel_init()
{
    console_init();
    gdt_init();
    interrupt_init();
    unsigned int i = 1;
    DEBUGK("Hello, world! %#010x\n", i);
    asm volatile("int $0x80");
    // task_init();
}
