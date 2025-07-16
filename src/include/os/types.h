#ifndef OS_TYPES_H
#define OS_TYPES_H

#define EOF -1 // END OF FILE
#define NULL 0 // 空指针
#define EOS '\0' // 字符串结尾

#define bool _Bool
#define true 1
#define false 0

#define _packed __attribute__((packed)) // 用于定义特殊的结构体
#define _inline __attribute__((always_inline)) inline

typedef unsigned int size_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef uint32_t time_t;
typedef uint32_t idx_t;

typedef int32_t fd_t;
typedef enum std_fd_t
{
    stdin,
    stdout,
    stderr,
} std_fd_t;

#endif
