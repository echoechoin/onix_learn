#include "onix/interrupt.h"
#include "onix/global.h"
#include "onix/debug.h"
#include "onix/printk.h"
#include "onix/io.h"
#include "onix/types.h"
#include "onix/assert.h"
#include "onix/task.h"

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)
// #define LOGK(fmt, args...)
#define PIC_M_CTRL 0x20 // 主片的控制端口
#define PIC_M_DATA 0x21 // 主片的数据端口
#define PIC_S_CTRL 0xa0 // 从片的控制端口
#define PIC_S_DATA 0xa1 // 从片的数据端口
#define PIC_EOI 0x20    // 通知中断控制器中断结束
#define ENTRY_SIZE 0x30

uint32 counter = 0;
gate_t idt[IDT_SIZE] = {0};                       // 内核中断描述符表
pointer_t idt_ptr;                                // 内核中断描述符表指针
handler_t handler_table[IDT_SIZE] = {0};          // 中断处理函数表
extern handler_t handler_entry_table[ENTRY_SIZE]; // 在handler.asm中定义且初始化好的中断处理函数入口表

static char *messages[] = {
    "#DE Divide Error\0",
    "#DB RESERVED\0",
    "--  NMI Interrupt\0",
    "#BP Breakpoint\0",
    "#OF Overflow\0",
    "#BR BOUND Range Exceeded\0",
    "#UD Invalid Opcode (Undefined Opcode)\0",
    "#NM Device Not Available (No Math Coprocessor)\0",
    "#DF Double Fault\0",
    "    Coprocessor Segment Overrun (reserved)\0",
    "#TS Invalid TSS\0",
    "#NP Segment Not Present\0",
    "#SS Stack-Segment Fault\0",
    "#GP General Protection\0",
    "#PF Page Fault\0",
    "--  (Intel reserved. Do not use.)\0",
    "#MF x87 FPU Floating-Point Error (Math Fault)\0",
    "#AC Alignment Check\0",
    "#MC Machine Check\0",
    "#XF SIMD Floating-Point Exception\0",
    "#VE Virtualization Exception\0",
    "#CP Control Protection Exception\0",
};

static void exception_handler(
    int vector,
    uint32 edi, uint32 esi, uint32 ebp, uint32 esp,
    uint32 ebx, uint32 edx, uint32 ecx, uint32 eax,
    uint32 gs, uint32 fs, uint32 es, uint32 ds,
    uint32 vector0, uint32 error, uint32 eip, uint32 cs, uint32 eflags)
{
    char *message = NULL;
    if (vector < 22)
    {
        message = messages[vector];
    }
    else
    {
        message = messages[15];
    }

    printk("Exception : [0x%02X] %s \n", vector, messages[vector]);
    printk("   VECTOR : 0x%02X\n", vector);
    printk("    ERROR : 0x%08X\n", error);
    printk("   EFLAGS : 0x%08X\n", eflags);
    printk("       CS : 0x%02X\n", cs);
    printk("      EIP : 0x%08X\n", eip);
    printk("      ESP : 0x%08X\n", esp);
    // 阻塞：如果不阻塞将会再次调用异常触发的指令导致再次产生异常
    while (true)
        ;
}

// 通知中断控制器，中断处理结束
void send_eoi(int vector)
{
    // 主片只需要向主片的端口发送EOI
    if (vector >= 0x20 && vector < 0x28)
    {
        outb(PIC_M_CTRL, PIC_EOI);
    }
    // 从片需要向从片和主片的端口都发送EOI

    if (vector >= 0x28 && vector < 0x30)
    {
        outb(PIC_M_CTRL, PIC_EOI);
        outb(PIC_S_CTRL, PIC_EOI);
    }
}

void set_interrupt_handler(uint32 irq, handler_t handler)
{
    assert(irq >= 0 && irq < 16);
    handler_table[IRQ_MASTER_NR + irq] = handler;
}

void set_interrupt_mask(uint32 irq, bool enable)
{
    assert(irq >= 0 && irq < 16);
    uint16 port;
    // 表示该中断是主片的中断
    if (irq < 8)
    {
        port = PIC_M_DATA;
    }
    // 表示该中断是从片的中断
    else
    {
        port = PIC_S_DATA;
        irq -= 8;
    }
    if (enable)
    {
        // 使能中断
        outb(port, inb(port) & ~(1 << irq));
    }
    else
    {
        // 禁止中断
        outb(port, inb(port) | (1 << irq));
    }
}

static void default_handler(
    int vector,
    uint32 edi, uint32 esi, uint32 ebp, uint32 esp,
    uint32 ebx, uint32 edx, uint32 ecx, uint32 eax,
    uint32 gs, uint32 fs, uint32 es, uint32 ds,
    uint32 vector0, uint32 error, uint32 eip, uint32 cs, uint32 eflags)
{
    send_eoi(vector);
    DEBUGK("[%x] default interrupt called %d...\n", vector, counter);
}

static void pic_init()
{
    // ICW需要按照顺序写入，否者中断控制器不知道如何处理
    outb(PIC_M_CTRL, 0b00010001); // ICW1: 边沿触发, 级联 8259, 需要ICW4.
    outb(PIC_M_DATA, 0x20);       // ICW2: 起始中断向量号 0x20
    outb(PIC_M_DATA, 0b00000100); // ICW3: IR2接从片.
    outb(PIC_M_DATA, 0b00000001); // ICW4: 8086模式, 正常EOI

    outb(PIC_S_CTRL, 0b00010001); // ICW1: 边沿触发, 级联 8259, 需要ICW4.
    outb(PIC_S_DATA, 0x28);       // ICW2: 起始中断向量号 0x28
    outb(PIC_S_DATA, 2);          // ICW3: 设置从片连接到主片的 IR2 引脚
    outb(PIC_S_DATA, 0b00000001); // ICW4: 8086模式, 正常EOI

    outb(PIC_M_DATA, 0b11111111); // 关闭主片所有中断
    outb(PIC_S_DATA, 0b11111111); // 关闭从片所有中断
}

static void idt_init()
{
    for (size_t i = 0; i < ENTRY_SIZE; i++)
    {
        gate_t *gate = &idt[i];
        handler_t handler = handler_entry_table[i];

        gate->offset0 = (uint32)handler & 0xffff;
        gate->offset1 = ((uint32)handler >> 16) & 0xffff;
        gate->selector.index = 1; // 代码段
        gate->reserved = 0;       // 保留不用
        gate->type = 0b1110;      // 中断门：会将eflags寄存器的IF位置0， 陷阱门则不会
        gate->segment = 0;        // 系统段
        gate->DPL = 0;            // 内核态
        gate->present = 1;        // 有效
    }

    // 初始化异常处理默认函数
    for (size_t i = 0; i < 0x20; i++)
    {
        handler_table[i] = exception_handler;
    }

    // 初始化外部中断处理默认函数
    for (size_t i = 0x20; i < ENTRY_SIZE; i++)
    {
        handler_table[i] = default_handler;
    }


    idt_ptr.base = (uint32)idt;
    idt_ptr.limit = sizeof(idt) - 1;
    asm volatile("lidt idt_ptr\n");
}

void interrupt_init()
{
    pic_init();
    idt_init();
}
