#ifndef ONIX_STDLIB_H
#define ONIX_STDLIB_H

#include <os/types.h>

void delay(uint32_t count);
void hang();

uint8_t bcd_to_bin(uint8_t value);
uint8_t bin_to_bcd(uint8_t value);

#endif