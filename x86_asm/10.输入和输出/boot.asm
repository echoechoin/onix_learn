; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

CRT_ADDR_REG    equ 0x3D4
CRT_DATA_REG    equ 0x3D5
CRT_CURSOR_HIGH equ 0x0E
CRT_CURSOR_LOW  equ 0x0F

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

mov ax, 0xb800
mov es, ax

mov si, message
mov di, 0

call print

set_cursor_pos:
    ; 设置光标位置，传入参数在ax中
    ; 设置光标的低8位
    push dx
    push bx
    mov bx, ax
    mov dx, CRT_ADDR_REG
    mov al, CRT_CURSOR_LOW
    out dx, al

    mov dx, CRT_DATA_REG
    mov al, bl
    out dx, al

    ; 设置光标的高8位
    mov dx, CRT_ADDR_REG
    mov al, CRT_CURSOR_HIGH
    out dx, al

    mov dx, CRT_DATA_REG
    mov al, bh
    out dx, al

    pop bx
    pop dx
    ret

get_cursor_pos:
    ; 获取光标位置，返回值存储在ax中
    push dx
    ; 获取光标的高8位
    mov dx, CRT_ADDR_REG
    mov al, CRT_CURSOR_HIGH
    out dx, al

    mov dx, CRT_DATA_REG
    in al, dx

    shl ax, 8

    ; 获取光标的低8位
    mov dx, CRT_ADDR_REG
    mov al, CRT_CURSOR_LOW
    out dx, al

    mov dx, CRT_DATA_REG
    in al, dx

    pop dx
    ret

print:
    ; 获取光标位置
    call get_cursor_pos

    ; 获取显存位置
    mov di, ax
    shl di, 1 ; di = di * 2

    ; 设置显存中的内容
    mov bl, [si]
    mov bh, 0b00001100; 黑底红字
    
    cmp bl, 0
    jz print_end
    mov word [es:di], bx

    ; 设置显存内容
    inc si
    ; 设置光标位置
    inc ax
    mov bx, ax
    call set_cursor_pos
    jmp print

print_end:

halt:
    ; hlt ; 暂停CPU，等待中断的发生
    jmp halt

message:
    db "hello world!!!", 0

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa