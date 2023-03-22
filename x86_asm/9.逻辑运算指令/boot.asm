; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10




halt:
    hlt ; 暂停CPU，等待中断的发生
    jmp halt

; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa