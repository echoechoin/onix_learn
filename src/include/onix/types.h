#ifndef ONIX_TYPES_H
#define ONIX_TYPES_H

#define EOF -1 // END OF FILE
#define EOS '\0' // END OF STRING
#define NULL ((void*)0) // 空指针

#ifndef _Bool
#define _Bool int
#endif

#define bool _Bool
#define true 1
#define false 0

#define _packed __attribute__((packed))
#define _ofp __attribute__((optimize("omit-frame-pointer")))

typedef unsigned int       size_t;
typedef char               int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

// 时间戳
typedef uint32 time_t;

// index
typedef uint32 idx_t;
#endif
