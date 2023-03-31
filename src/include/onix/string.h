#ifndef ONIX_STRING_H
#define ONIX_STRING_H

#include "onix/types.h"

char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);
size_t strlen(const char *str);
int strcmp(const char *lhs, const char *rhs);
char *strchr(const char *str, int ch);
char *strrchr(const char *str, int ch);

int memcmp(const void *lhs, const void *rhs, size_t n);
void *memset(void *dest, int ch, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memchr(const void *str, int ch, size_t n);

char *strncpy(char *dest, const char *src, size_t n);

#endif