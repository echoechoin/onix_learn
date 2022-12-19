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
    pushad ; 将ax cx dx bx sp bp si di寄存器的值压入栈中
    ; pusha只会压入16位寄存器，而pushad会压入32位寄存器
    ; 1. 设置扇区数量为1个
    mov dx, 0x1f2
    mov al, bl
    out dx, al
    ; 2. 设置从第ecx个扇区开始读
    mov al, cl
    mov dx, 0x1f3
    out dx, al ; 起始扇区位置低八位

    shr ecx, 8
    mov dx, 0x1f4
    mov al, cl
    out dx, al ; 起始扇区位置中间八位

    shr ecx, 8
    mov dx, 0x1f5
    mov al, cl
    out dx, al ; 起始扇区位置高八位

    shr ecx, 8
    and cl, 0b00001111 

    ; 3. 设置模式为LBA模式, 从ata0的主盘读
    mov dx, 0x1f6
    mov al, 0b1110_0000
    or al, cl ; 起始扇区位置最后高四位
    out dx, al

    ; 4. 设置为读硬盘
    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    .check:
        ; ata要求检查状态前需要一点点的延迟
        nop
        nop
        nop
        ; 5. 判断硬盘是否准备好
        in al, dx
        and al, 0b1000_1000 ; 第7位表示硬盘是否忙, 第3位表示硬盘是否准备好
        cmp al, 0b0000_1000 ; 检查是否可读
        jnz .check

    ; 6. 读取数据
    xor eax, eax
    mov al, bl
    mov dx, 256
    mul dx ; ax = ax * dx = bl * 256


    mov dx, 0x1f0
    mov cx, ax ; 循环次数

    .readw:
        in ax, dx ; 读取一个word
        nop
        nop
        nop
        mov [edi], ax ; 将读取的数据写入内存
        add edi, 2    ; 下一个字的地址
        loop .readw   ; 循环读取， 直到cx为0
    popad
    ret


; 字符填充
times 510-($-$$) db 0

; 主引导记录结束标记
db 0x55, 0xaa
