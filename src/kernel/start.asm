[bits 32]

magic         equ 0xe85250d6
architecture  equ 0
header_length equ header_end - header_start

; multiboot2 头部 refer to https://www.gnu.org/software/grub/manual/multiboot2/multiboot.pdf
section .multiboot2
header_start:
        dd magic                                   ; multiboot 魔数
        dd architecture                            ; i386 架构
        dd header_length                           ; 头部长度
        dd -(magic + architecture + header_length) ; 校验和: 和`magic`，`arch`，`header_length`的和为0

        ; 结束标志，作爲終止標籤的最後一個標籤，其類型爲0、大小爲8
        dw 0 ; type等于0表示结束
        dw 0 ; 没有flags
        dd 8 ; size
header_end:


section .text
extern console_init
extern kernel_init
extern memory_init
extern gdt_init
extern tss_init
extern gdt_ptr

code_selector equ (1 << 3)
data_selector equ (2 << 3)


global _start
_start:
        mov esp, 0x10000
        push ebx ; ards_count 的内存位置 / multiboot2 头部的内存位置
        push eax ; 内核魔数 / multiboot2魔数
        call console_init
        call gdt_init
        lgdt [gdt_ptr]
        jmp code_selector:_next ; 跳转到_next，跳转的目的是为了加载data_selector

_next:
        ; 加载data_selector
        call tss_init
        mov ax, data_selector
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        call memory_init
        ; 修改栈指针
        mov esp, 0x10000
        ; 跳转到内核
        call kernel_init
        jmp $