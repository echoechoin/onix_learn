#ifndef OS_CLOCK_H
#define OS_CLOCK_H

#define PIT_CHAN0_REG 0X40
#define PIT_CHAN2_REG 0X42
#define PIT_CTRL_REG 0X43

// 时钟中断频率
#define HZ 100

// 计数器频率 1s 计数器会减这么多次
#define OSCILLATOR 1193182

// 表示 10ms 触发一次中断
#define CLOCK_COUNTER (OSCILLATOR / HZ)

// 表示我们系统时间片为 10ms
#define JIFFY (1000 / HZ)

void clock_init();

#endif
