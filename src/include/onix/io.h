#ifndef ONIX_IO_H
#define ONIX_IO_H

#include "onix/types.h"

// 输入一个字节
extern uint8 inb(uint16 port);
// 输入一个字
extern uint16 inw(uint16 port);

// 输出一个字节
extern void outb(uint16 port, uint8 value);
// 输出一个字
extern void outw(uint16 port, uint16 value);

#endif