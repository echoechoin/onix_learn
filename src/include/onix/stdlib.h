#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <onix/types.h>

#define MAX(a, b) (a < b ? b : a)
#define MIN(a, b) (a < b ? a : b)

uint8 bcd_to_bin(uint8 value);
uint8 bin_to_bcd(uint8 value);

uint32 div_round_up(uint32 num, uint32 size);

#endif