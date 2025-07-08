[bits 32] ; 指令集切换

global _start ; 全局符号定义, 链接器会将_start 标记为程序执行的起点
_start:
    mov byte [0xb8000], 'K'; 表示进入了内核
    jmp $; 阻塞
