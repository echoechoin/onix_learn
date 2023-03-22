; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

offset equ 0x0000 ; 类似于宏定义，用于定义一个常量
data equ 0x55aa

; 直接寻址
mov ax, 0x1000
mov ds, ax

;立即数寻址
mov ax, [offset]
mov [offset], ax
; mov [offset], 0x10 ; 0x10是立即数，不能直接寻址, 需要指定大小
mov byte [offset], 0x10 ; 这样就可以了
mov word [offset], 0x10
mov dword [offset], 0x10

; 寄存器间接寻址，只有bx bp si di可以用于间接寻址
mov ax, [bx] ; ds << 4 + bx
mov ax, [bp] ; ss << 4 + bp
mov ax, [si] ; ds << 4 + si
mov ax, [di] ; ds << 4 + di

; 基址 + 变址寻址
mov ax, [bx + si + 0x100] ; ds
mov ax, [bp + di + 0x100] ; ss

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa