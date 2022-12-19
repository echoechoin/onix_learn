[org 0x7c00]

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

; 0xb8000是显存的起始地址 这里是直接写入显存来显示字符
; mov ax, 0xb800
; mov ds, ax
; mov ds, ax
; mov byte [20], 'H'


; 这里使用的是int 0x10中断显示字符
mov si, booting
call print

mov edi, 0x1000      ; 目的地址
mov ecx, 1           ; 起始扇区: loader.img
mov bl,  4           ; 读取4个扇区 最大为256
call read_disk

cmp word [0x1000], 0x55aa ; 相等则标志位ZF=0
jnz error ; 如果ZF=1则跳转到error

jmp 0x1000

; 无限循环 $ 表示当前地址
jmp $

error:
        mov si, .msg
        call print
        hlt       ; 让CPU停止运行
        jmp $
        .msg db "Booting Error!", 10, 13, 0 ; \n\r\0

booting:
        db "Booting Onix...", 10, 13, 0 ; \n\r\0

print:
        mov ah, 0x0e ; 0x0e是显示字符的功能号 
.next:
        mov al, [si] ; 从si处取出字符
        cmp al, 0    ; 比较是否到了结尾
        jz .done     ; 如果到了结尾就跳转到.done
        int 0x10     ; 否则就显示字符
        inc si       ; si指向下一个字符
        jmp .next    ; 继续显示
.done:
        ret


read_disk:
        ; 读取硬盘
        ; edi: 存储内存位置
        ; ecx: 存储起始扇区位置
        ; bl:  存储扇区数量


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


; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa
