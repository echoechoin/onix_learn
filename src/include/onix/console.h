#ifndef ONIX_CONSOLE_H
#define ONIX_CONSOLE_H

#include "onix/types.h"
#include "onix/io.h"
#include "onix/string.h"

void console_init(void);
void console_clear(void);
void console_write(char *buf, uint32 count);

#endif