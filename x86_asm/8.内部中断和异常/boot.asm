; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

; 注册中断服务程序
mov word [0x80 *4], print
mov word [0x80 *4 + 2], cs

mov word [0x00 *4], print
mov word [0x00 *4 + 2], cs

xchg bx, bx

; 1. 软中断
int 0x80

; 2. 除零异常
mov ax, 0
div ax ; 除零异常结束后会再次执行此指令，导致死循环不停调用中断服务程序

halt:
    hlt ; 暂停CPU，等待下一个中断
    jmp halt

video:
    dw 0x0

print:
    push ax
    push bx
    push es
    mov ax, 0xb800
    mov es, ax
    mov bx, [video]
    mov byte [es:bx], '.'
    add word [video], 2
    pop es
    pop bx
    pop ax
    xchg bx, bx
    ; 中断函数返回: 将栈顶的ip弹出，然后跳转到这个地址
    iret

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa