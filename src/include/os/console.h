#ifndef OS_CONSOLE_H
#define OS_CONSOLE_H

#include <os/types.h>

void console_init();
void console_clear();
int32_t console_write(char *buf, uint32_t count);

#endif