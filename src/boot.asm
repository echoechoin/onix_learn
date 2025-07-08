[org 0x7c00]				; 指定程序的起始地址, 后续jmp等命令都会添加上这个偏移; 后续直接将此代码加载阿斗0x7c00这个位置运行

mov ax, 3					; 设置屏幕模式为文本模式，清除屏幕（ax寄存器可以用于 BIOS 中断调用参数）
int 0x10					; 使用BIOS中断设置

; 初始化段寄存器
mov ax, 0					; 通用数据存储；算术和逻辑运；BIOS 中断调用参数；数据传输
mov ds, ax					; 数据段寄存器 实模式下的内存寻址
mov es, ax					; 额外数据段寄存器 字符串操作(movsb等); 实模式下的内存寻址(mov byte [es:0], 'H')
mov ss, ax
mov sp, 0x7c00   			; 栈从高字节向低字节递减

; 读取磁盘
mov edi, 0x1000; 读取的目标内存
mov ecx, 2; 起始扇区
mov bl, 3; 扇区数量
call read_disk

; 检查磁盘读取内容是否正确
cmp word [0x1000], 0x55aa
jnz error
jmp 0:0x1002                ; 跳转到0x1002

;;;;;;;;;;;;;;;;;;;;;;;
; bl 读取扇区数量
; ecx 起始扇区
; edi 目标内存
;;;;;;;;;;;;;;;;;;;;;;;
read_disk:

    ; 设置读写扇区的数量
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    inc dx; 0x1f3
    mov al, cl; 起始扇区的前八位
    out dx, al

    inc dx; 0x1f4
    shr ecx, 8
    mov al, cl; 起始扇区的中八位
    out dx, al

    inc dx; 0x1f5
    shr ecx, 8
    mov al, cl; 起始扇区的高八位
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111; 将高四位置为 0

    mov al, 0b1110_0000;
    or al, cl
    out dx, al; 主盘 - LBA 模式

    inc dx; 0x1f7
    mov al, 0x20; 读硬盘
    out dx, al

    xor ecx, ecx; 将 ecx 清空
    mov cl, bl; 得到读写扇区的数量

    .read:
        push cx; 保存 cx
        call .waits; 等待数据准备完毕
        call .reads; 读取一个扇区
        pop cx; 恢复 cx
        loop .read

    ret

    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx
            jmp $+2; nop 直接跳转到下一行
            jmp $+2; 一点点延迟
            jmp $+2
            and al, 0b1000_1000
            cmp al, 0b0000_1000
            jnz .check
        ret

    .reads:
        mov dx, 0x1f0
        mov cx, 256; 一个扇区 256 字
        .readw:
            in ax, dx
            jmp $+2; 一点点延迟
            jmp $+2
            jmp $+2
            mov [edi], ax
            add edi, 2
            loop .readw
        ret

write_disk:

    ; 设置读写扇区的数量
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    inc dx; 0x1f3
    mov al, cl; 起始扇区的前八位
    out dx, al

    inc dx; 0x1f4
    shr ecx, 8
    mov al, cl; 起始扇区的中八位
    out dx, al

    inc dx; 0x1f5
    shr ecx, 8
    mov al, cl; 起始扇区的高八位
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111; 将高四位置为 0

    mov al, 0b1110_0000;
    or al, cl
    out dx, al; 主盘 - LBA 模式

    inc dx; 0x1f7
    mov al, 0x30; 写硬盘
    out dx, al

    xor ecx, ecx; 将 ecx 清空
    mov cl, bl; 得到读写扇区的数量

    .write:
        push cx; 保存 cx
        call .writes; 写一个扇区
        call .waits; 等待硬盘繁忙结束
        pop cx; 恢复 cx
        loop .write

    ret

    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx
            jmp $+2; nop 直接跳转到下一行
            jmp $+2; 一点点延迟
            jmp $+2
            and al, 0b1000_0000
            cmp al, 0b0000_0000
            jnz .check
        ret

    .writes:
        mov dx, 0x1f0
        mov cx, 256; 一个扇区 256 字
        .writew:
            mov ax, [edi]
            out dx, ax
            jmp $+2; 一点点延迟
            jmp $+2
            jmp $+2
            add edi, 2
            loop .writew
        ret

print:
    mov ah, 0x0e
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

error:
    mov si, .msg
    call print
    hlt; 让 CPU 停止
    jmp $
    .msg db "Booting Error!!!", 10, 13, 0

times 510 - ($ - $$) db 0	; 填充 0, 直到填充到 510 字节

; 主引导扇区的最后两个字节必须是 0x55 0xaa
; dw 0xaa55 ; 大端字节序
db 0x55, 0xaa ; 实际存到硬盘的是小端字节序
