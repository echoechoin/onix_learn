#include "onix/io.h"
#include "onix/interrupt.h"
#include "onix/assert.h"
#include "onix/debug.h"
#include "onix/clock.h"
#include "onix/task.h"

// 定时器端口
#define PIT_CHAN0_REG 0X40 // 8253/8254 PIT的通道0的端口号，用于设置计数器的初始值
#define PIT_CHAN2_REG 0X42 // 8253/8254 PIT的通道2的端口号，用于设置计数器的初始值
#define PIT_CTRL_REG 0X43  // 8253/8254 PIT的控制端口号，用于设置计数器的工作模式

// beep
#define SPEAKER_REG 0x61                    // 复用了键盘的端口，用于控制PC喇叭的开关
#define BEEP_HZ 440                         // 蜂鸣器的频率
#define BEEP_COUNTER (OSCILLATOR / BEEP_HZ) // 蜂鸣器的计数器初始值
uint32 volatile beeping = 0;

// 时钟中断
#define HZ 100                              // 中断发生的频率
#define OSCILLATOR 1193182                  // 定时器的频率
#define CLOCK_COUNTER (OSCILLATOR / HZ)     // 计数器减 CLOCK_COUNTER 次发生一次中断
#define JIFFY (1000 / HZ)                   // 中断发生的时间间隔（0.01s） jiffy：瞬间
uint32 volatile jiffies = 0;                // 时间片计数器 = 产生了多少个中断 * 时间片长度
uint32 jiffy = JIFFY;                       // 时间片长度

static void stop_beep()
{
    // 5次计数器中断后关闭蜂鸣器
    if (beeping && jiffies > beeping)
    {
        // 关闭蜂鸣器和计数器2
        outb(SPEAKER_REG, inb(SPEAKER_REG) & 0b11111100);
        beeping = 0;
    }
}

static void clock_handler(int vector)
{
    assert(vector == 0x20);
    send_eoi(vector);
    if (jiffies % 200 == 0)
    {
        start_beep();
    }
    schedule();  // 进程调度
    jiffies++;   // 产生中断的次数++
    // DEBUGK("clock jiffies %d ...\n", jiffies);
    stop_beep(); // 每次中断都判断一下是否需要关闭蜂鸣器。
}

static void pit_init()
{
    // 00：初始化计数器0
    // 11：先写低字节，再写高字节
    // x10：模式2，计数器清零时发生中断，然后重新装载初始值
    outb(PIT_CTRL_REG, 0b00110100);
    // 写入CLOCK_COUNTER 范围是0~65535
    outb(PIT_CHAN0_REG, CLOCK_COUNTER & 0xff);
    outb(PIT_CHAN0_REG, (CLOCK_COUNTER >> 8) & 0xff);


    // 配置计数器 2 蜂鸣器
    // 00：初始化计数器2
    // 11：先写低字节，再写高字节
    // x11：模式3，方波发生器
    outb(PIT_CTRL_REG, 0b10110110);
    // 写入BEEP_COUNTER 范围是0~65535
    outb(PIT_CHAN2_REG, (uint8)BEEP_COUNTER);
    outb(PIT_CHAN2_REG, (uint8)(BEEP_COUNTER >> 8));
}

// TODO: 在我的虚拟机里不会响，不知道为什么
void start_beep()
{
    if (!beeping)
    {
        // 开启蜂鸣器和计数器2
        outb(SPEAKER_REG, inb(SPEAKER_REG) | 0b00000011);
    }
    // 等到5次中断后关闭蜂鸣器
    beeping = jiffies + 5;
}

void clock_init()
{
    pit_init();
    set_interrupt_handler(IRQ_CLOCK, clock_handler);
    set_interrupt_mask(IRQ_CLOCK, true);
}
