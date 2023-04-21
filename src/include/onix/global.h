#ifndef ONIX_GLOBAL_H
#define ONIX_GLOBAL_H

#include "onix/types.h"

#define GDT_SIZE 128

#define KERNEL_CODE_IDX 1 // 内核代码段索引
#define KERNEL_DATA_IDX 2 // 内核数据段索引
#define KERNEL_TSS_IDX  3 // 任务状态段索引
#define USER_CODE_IDX   4 // 用户代码段索引
#define USER_DATA_IDX   5 // 用户数据段索引

#define KERNEL_TSS_SELECTOR (KERNEL_TSS_IDX << 3)

#define KERNEL_CODE_SELECTOR (KERNEL_CODE_IDX << 3) // 表示内核代码段的特权级是0
#define KERNEL_DATA_SELECTOR (KERNEL_DATA_IDX << 3) // 表示内核数据段的特权级是0
#define USER_CODE_SELECTOR (USER_CODE_IDX << 3 | 0b011) // 表示用户代码段的特权级是3
#define USER_DATA_SELECTOR (USER_DATA_IDX << 3 | 0b011) // 表示用户数据段的特权级是3

// 全局描述符
typedef struct descriptor_t /* 共 8 个字节 */
{
    unsigned short limit_low;      // 段界限 0 ~ 15 位
    unsigned int base_low : 24;    // 基地址 0 ~ 23 位 16M
    unsigned char type : 4;        // 段类型
    unsigned char segment : 1;     // 1 表示代码段或数据段，0 表示系统段
    unsigned char DPL : 2;         // Descriptor Privilege Level 描述符特权等级 0 ~ 3
    unsigned char present : 1;     // 存在位，1 在内存中，0 在磁盘上
    unsigned char limit_high : 4;  // 段界限 16 ~ 19;
    unsigned char available : 1;   // 该安排的都安排了，送给操作系统吧
    unsigned char long_mode : 1;   // 64 位扩展标志
    unsigned char big : 1;         // 32 位 还是 16 位;
    unsigned char granularity : 1; // 粒度 4KB 或 1B
    unsigned char base_high;       // 基地址 24 ~ 31 位
} _packed descriptor_t;

// 段选择子
typedef struct selector_t
{
    uint8 RPL : 2; // Descriptor Privilege Level 段描述符请求特权级
    uint8 TI : 1; // 0 表示 GDT，1 表示 LDT
    uint16 index : 13; // 8192 个描述符
} selector_t;

// 全局描述符表指针
typedef struct pointer_t
{
    uint16 limit;
    uint32 base;
} _packed pointer_t;

typedef struct tss_t
{
    uint32 backlink;      // reserved
    uint32 esp0;          // **ring0 的栈顶地址**
    uint32 ss0;           // **ring0 的栈段选择子**
    uint32 esp1;          // reserved
    uint32 ss1;           // reserved
    uint32 esp2;          // reserved
    uint32 ss2;           // reserved
    uint32 cr3;           // reserved
    uint32 eip;           // reserved
    uint32 flags;         // reserved
    uint32 eax;           // reserved
    uint32 ecx;           // reserved
    uint32 edx;           // reserved
    uint32 ebx;           // reserved
    uint32 esp;           // reserved
    uint32 ebp;           // reserved
    uint32 esi;           // reserved
    uint32 edi;           // reserved
    uint32 es;            // reserved
    uint32 cs;            // reserved
    uint32 ss;            // reserved
    uint32 ds;            // reserved
    uint32 fs;            // reserved
    uint32 gs;            // reserved
    uint32 ldtr;          // reserved
    uint16 trace : 1;     // reserved
    uint16 reversed : 15; // reserved
    uint16 iobase;        // reserved
    uint32 ssp;           // reserved
} _packed tss_t;

void gdt_init();
void tss_init();

#endif