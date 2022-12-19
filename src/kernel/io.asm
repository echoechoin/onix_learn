[bits 32]

section .text

global inb
inb:
        push ebp
        mov ebp, esp
        ; 栈结构
        ; ebp
        ; eip
        ; port

        xor eax, eax
        mov edx, [ebp + 8]; port
        in al, dx; 将端口号dx的8bit数据从端口读入到al中
        jmp $+2; 一些延迟
        jmp $+2
        jmp $+2

        leave
        ret

global outb
outb:
        push ebp
        mov ebp, esp
        ; 栈结构
        ; ebp
        ; eip
        ; port
        ; value

        mov edx, [ebp + 8]; port
        mov eax, [ebp + 12]; value
        out dx, al; 将al中的8bit数据写入到端口号dx中
        jmp $+2; 一些延迟
        jmp $+2
        jmp $+2

        leave
        ret

global inw
inw:
        push ebp
        mov ebp, esp

        xor eax, eax
        mov edx, [ebp + 8]; port
        in ax, dx; 将端口号dx的16bit数据从端口读入到ax中
        jmp $+2; 一些延迟
        jmp $+2
        jmp $+2

        leave
        ret

global outw
outw:
        push ebp
        mov ebp, esp

        mov edx, [ebp + 8]; port
        mov eax, [ebp + 12]; value
        out dx, ax; 将ax中的16bit数据写入到端口号dx中
        jmp $+2; 一些延迟
        jmp $+2
        jmp $+2

        leave
        ret
