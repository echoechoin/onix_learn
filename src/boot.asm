[org 0x7c00]				; 指定程序的起始地址, 后续jmp等命令都会添加上这个偏移; 后续直接将此代码加载阿斗0x7c00这个位置运行

mov ax, 3					; 设置屏幕模式为文本模式，清除屏幕（ax寄存器可以用于 BIOS 中断调用参数）
int 0x10					; 使用BIOS中断设置

; 初始化段寄存器
mov ax, 0					; 通用数据存储；算术和逻辑运；BIOS 中断调用参数；数据传输
mov ds, ax					; 数据段寄存器 实模式下的内存寻址
mov es, ax					; 额外数据段寄存器 字符串操作(movsb等); 实模式下的内存寻址(mov byte [es:0], 'H')
mov ss, ax
mov sp, 0x7c00   			; 栈从高字节向低字节递减

; 不再直接操作显存
; mov ax, 0xb800				; 0xb8000 文本显示器的内存区域
; mov ds, ax					; 写入到段寄存器ds，在实模式下，DS 的值会影响内存寻址

; 而使用BIOS中断打印启动日志
mov si, booting             ; si 寄存器通常用于字符串操作
call print

jmp $						; 跳转到当前地址

print:
    mov ah, 0x0e    ; 设置为打印字符串功能
.next:
    mov al, [si]    ; 输入字符，间接寻址，告诉汇编器，SI 的值不是操作数本身，而是获取si
    cmp al, 0       ; 表示到字符串结尾
    jz .done
    int 0x10        ; 输出字符，光标位置修改
    inc si          ; si++
    jmp .next
.done:
    ret

; 存放booting字符串
booting:
    db "Booting OS...", 10, 13, 0 ; \n\r

times 510 - ($ - $$) db 0	; 填充 0, 直到填充到 510 字节

; 主引导扇区的最后两个字节必须是 0x55 0xaa
; dw 0xaa55 ; 大端字节序
db 0x55, 0xaa ; 实际存到硬盘的是小端字节序
