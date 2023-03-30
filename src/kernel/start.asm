[bits 32]

extern console_init
extern kernel_init
extern memory_init
extern gdt_init

global _start
_start:
        push ebx ; ards_count 的内存位置
        push eax ; 内核魔数
        call console_init
        call gdt_init
        call memory_init
        call kernel_init
        int 0x10 ; 触发异常
        jmp $