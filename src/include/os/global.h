#ifndef ONIX_GLOBAL_H
#define ONIX_GLOBAL_H

#include <os/types.h>

#define GDT_SIZE 128

#define KERNEL_CODE_IDX 1
#define KERNEL_DATA_IDX 2
#define KERNEL_TSS_IDX 3

#define USER_CODE_IDX 4
#define USER_DATA_IDX 5

#define KERNEL_CODE_SELECTOR (KERNEL_CODE_IDX << 3)
#define KERNEL_DATA_SELECTOR (KERNEL_DATA_IDX << 3)
#define KERNEL_TSS_SELECTOR (KERNEL_TSS_IDX << 3)

#define USER_CODE_SELECTOR (USER_CODE_IDX << 3 | 0b11)
#define USER_DATA_SELECTOR (USER_DATA_IDX << 3 | 0b11)

// 全局描述符
typedef struct descriptor /* 共 8 个字节 */
{
    unsigned short limit_low;      // 段界限 0 ~ 15 位: 也就是内存界限，只有19位 因此每位表示4KB, 要设置4G 的内存界限，全设置为1即可，但是需要 - 1,因为界限 = 长度 - 1
    unsigned int base_low : 24;    // 基地址 0 ~ 23 位: 内存开始的位置；设置为0，之后实模式和BISO里的内容将不再存在
    unsigned char type : 4;        // 段类型: 
    unsigned char segment : 1;     // 1 表示代码段或数据段，0 表示系统段: 设置为1 表示我们要设置代码段
    unsigned char DPL : 2;         // Descriptor Privilege Level 描述符特权等级 0 ~ 3: 设置为0 表示最高特权
    unsigned char present : 1;     // 存在位，1 在内存中，0 在磁盘上; 我们暂时设置为1即可
    unsigned char limit_high : 4;  // 段界限 16 ~ 19;
    unsigned char available : 1;   // 该安排的都安排了，送给操作系统吧，对于CPU来说没用
    unsigned char long_mode : 1;   // 64 位扩展标志 设置为 0 即可
    unsigned char big : 1;         // 32 位 还是 16 位; 设置为1表示 wield 32位
    unsigned char granularity : 1; // 粒度 4KB 或 1B 设置为1 表示为4K
    unsigned char base_high;       // 基地址 24 ~ 31 位
} __attribute__((packed)) descriptor_t;

// 段选择子
typedef struct selector_t
{
    uint8_t RPL : 2;
    uint8_t TI : 1;
    uint16_t index : 13;
} selector_t;

// 全局描述符表指针
typedef struct pointer_t
{
    uint16_t limit; // 全描述符界限 (size - 1)
    uint32_t base;  // 全描述符基地址
} _packed pointer_t;

typedef struct tss_t
{
    uint32_t backlink; // 前一个任务的链接，保存了前一个任状态段的段选择子
    uint32_t esp0;     // ring0 的栈顶地址
    uint32_t ss0;      // ring0 的栈段选择子
    uint32_t esp1;     // ring1 的栈顶地址
    uint32_t ss1;      // ring1 的栈段选择子
    uint32_t esp2;     // ring2 的栈顶地址
    uint32_t ss2;      // ring2 的栈段选择子
    uint32_t cr3;
    uint32_t eip;
    uint32_t flags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtr;          // 局部描述符选择子
    uint16_t trace : 1;     // 如果置位，任务切换时将引发一个调试异常
    uint16_t reversed : 15; // 保留不用
    uint16_t iobase;        // I/O 位图基地址，16 位从 TSS 到 IO 权限位图的偏移
    uint32_t ssp;           // 任务影子栈指针
} _packed tss_t;

#define GDT_SECTOR_NUM(pointer) (((pointer)->limit + 1 / sizeof(descriptor_t))) // 128 个描述符

void gdt_init();
void show_gdt_list();

#endif