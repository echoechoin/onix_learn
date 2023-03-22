; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

PIC_M_CMD equ 0x20 ; 主片控制端口
PIC_M_DATA equ 0x21 ; 主片数据端口

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

; 注册时钟中断函数到0x08号中断向量
mov word [0x08 * 4], clock
mov word [0x08 * 4 + 2], cs

; 向OCW1写入屏蔽字，除了时钟中断，其他中断都屏蔽
mov al, 0xfe
out PIC_M_DATA, al

; 开启CPU中断总开关
sti

halt:
    mov bx, 3
    mov al, 'C'
    call blink
    jmp halt

clock:
    push bx
    push ax
    mov bx, 1
    mov al, 'C'
    call blink
    ; 向OCW2写入0x20，表示通知8259中断处理完毕
    mov al, 0x20
    out PIC_M_CMD, al

    pop ax
    pop bx
    iret

blink:
    push dx
    push es
    mov dx, 0xb800
    mov es, dx
    shl bx, 1; bx = bx * 2
    mov dl, [es:bx]
    cmp dl, ' '
    jnz .set_space
    mov [es:bx], al
    jmp .done
.set_space:
    mov byte [es:bx], ' '
.done:
    shr bx, 1; bx = bx / 2
    pop es
    pop dx
    ret


; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa