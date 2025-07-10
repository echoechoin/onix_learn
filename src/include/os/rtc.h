#ifndef OS_RTC_H
#define OS_RTC_H

#include "os/types.h"

void rtc_init();

uint8_t cmos_read(uint8_t addr);
void cmos_write(uint8_t addr, uint8_t value);

#endif