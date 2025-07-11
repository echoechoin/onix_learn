#include <os/stdlib.h>

void delay(uint32_t count)
{
    while (count--)
        ;
}

void hang()
{
    while (true)
        ;
}

// 将 bcd 码转成整数
uint8_t bcd_to_bin(uint8_t value)
{
    return (value & 0xf) + (value >> 4) * 10;
}

// 将整数转成 bcd 码
uint8_t bin_to_bcd(uint8_t value)
{
    return (value / 10) * 0x10 + (value % 10);
}

// 计算 num 分成 size 的数量
uint32_t div_round_up(uint32_t num, uint32_t size)
{
    return (num + size - 1) / size;
}
