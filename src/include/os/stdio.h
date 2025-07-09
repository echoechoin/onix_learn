#ifndef OS_STDIO_H
#define OS_STDIO_H

#include <os/stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char *buf, const char *fmt, ...);

#endif