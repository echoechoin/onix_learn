; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

; 初始化段寄存器
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax

; 初始化堆栈到0x7c00
mov sp, 0x7c00

; 0xb8000是显存的起始地址
mov ax, 0xb800
mov ds, ax
mov ds, ax
mov byte [0], 'H'
mov byte [1], 11111101b
mov byte [2], 'e'
mov byte [4], 'l'
mov byte [6], 'l'
mov byte [8], 'o'
mov byte [10], ' '
mov byte [12], 'W'
mov byte [14], 'o'
mov byte [16], 'r'
mov byte [18], 'l'
mov byte [20], 'd'
mov byte [22], '!'

; 无限循环 $ 表示当前地址
jmp $

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa