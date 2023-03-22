[bits 64]

extern printf
extern exit

section .text

global main
main:
    mov rax, 1 ; sys_write
    mov rdi, 1 ; stdout
    mov rsi, message
    mov rdx, message_end - message
    syscall
    ret

    ; 不调用退出会导致段错误
    mov rax, 60 ; sys_exit
    mov rdi, 0
    syscall

    

section .data
    message db "hello world", 10, 0
    message_end:

section .bss
    resb 0x100 ; reserve 0x100 bytes
    resw 0x100 ; reserve 0x200 bytes
    resd 0x100 ; reserve 0x400 bytes