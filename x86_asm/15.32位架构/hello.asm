[bits 32]

extern printf
extern exit

section .text
global main
main:
    push message
    call printf
    add esp, 4

    push 0
    call exit
    

section .data
    message db "hello world", 10, 0
    message_end: