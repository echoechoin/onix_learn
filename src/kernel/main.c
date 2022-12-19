#include "onix/onix.h"
#include "onix/types.h"
#include "onix/io.h"

#define CRT_ADDR_REG 0x3d4
#define CRT_DATA_REG 0x3d5

#define CRT_CURSOR_H 0x0e
#define CRT_CURSOR_L 0x0f

void kernel_init()
{
    // 将地址寄存器改为光标高地址寄存器
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    // 得到当前光标的位置的高字节
    uint16 pos = inb(CRT_DATA_REG) << 8;
    // 将地址寄存器改为光标低地址寄存器
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    // 得到当前光标的位置
    pos |= inb(CRT_DATA_REG);
}
