#include "onix/stdlib.h"

uint8 bcd_to_bin(uint8 value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}

uint8 bin_to_bcd(uint8 value)
{
    return ((value / 10) << 4) + (value % 10);
}

uint32 div_round_up(uint32 num, uint32 size)
{
    return (num + size - 1) / size;
}