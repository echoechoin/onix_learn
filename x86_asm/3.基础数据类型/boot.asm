; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

mov ax, 0xb800
mov es, ax

mov ax, 0
mov ds, ax
mov si, message
mov di, 0


mov cx, message_end - message

loop1:
    mov al, [ds:si]; al = 0*16 + message = 'H'
    mov [es:di], al; 0xb800 * 16 + 0 = 'H'
    inc si
    add di, 2
    loop loop1; 每次循环，cx减1，直到cx为0时，跳出循环

; 无限循环 $ 表示当前地址
jmp $

message: 
    db "Hello, World!", 0
message_end:

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa