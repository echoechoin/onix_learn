; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

; 加法
mov ax, 5
mov bx, 10
add ax, bx ; ax = ax + bx

add word [number], 1 ; number = number + 1

; 减法
mov ax, 5
mov bx, 10
sub ax, bx ; ax = ax - bx

; 乘法
mov ax, 5
mov bx, 10
mul bx ; dx : ax = ax * bx

; 除法
mov ax, 5
mov bx, 10
div bx ; ax = ax / bx, dx = ax % bx

number:
    dw 0x1234

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa