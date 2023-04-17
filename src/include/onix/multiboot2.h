#ifndef ONIX_MULTIBOOT2
#define ONIX_MULTIBOOT2

#include <onix/types.h>

// 进入内核时 eax 寄存器的值
#define MULTIBOOT2_MAGIC 0x36d76289

// multiboot2 使用 ebx 存储内存信息

// tag的类型
#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_MMAP 6

#define MULTIBOOT_MEMORY_AVAILABLE 1        // 可用
#define MULTIBOOT_MEMORY_RESERVED 2         // 保留
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3 // 高级电源管理可以使用的内存
#define MULTIBOOT_MEMORY_NVS 4              // 易失性内存
#define MULTIBOOT_MEMORY_BADRAM 5           // 损坏的内存

// multiboot tag
typedef struct multi_tag_t
{
    uint32 type; // tag的类型
    uint32 size;
    char data[0];
} multi_tag_t;

// multiboot mmap entry
typedef struct multi_mmap_entry_t
{
    uint64 addr;
    uint64 len;
    uint32 type;
    uint32 zero;
} multi_mmap_entry_t;

// multiboot mmap tag
typedef struct multi_tag_mmap_t
{
    uint32 type;
    uint32 size;
    uint32 entry_size;
    uint32 entry_version;
    multi_mmap_entry_t entries[0];
} multi_tag_mmap_t;


#endif
