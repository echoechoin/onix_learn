; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

mov ax, 0x1111
xchg bx, bx
mov bx, 0x2222
xchg bx, bx
mov cx, 0x3333
xchg bx, bx
mov dx, 0x4444
xchg bx, bx
mov di, 0x5555
xchg bx, bx
mov si, 0x6666
xchg bx, bx
mov bp, 0x7777
xchg bx, bx
mov sp, 0x8888
xchg bx, bx


; 无限循环 $ 表示当前地址
jmp $

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa