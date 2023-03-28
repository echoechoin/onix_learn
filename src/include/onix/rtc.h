#ifndef ONIX_RTC_H
#define ONIX_RTC_H

#include "onix/types.h"

uint8 cmos_read(uint8 addr);
void cmos_write(uint8 addr, uint8 value);

void rtc_init();
void set_alarm(uint32 secs);

#endif