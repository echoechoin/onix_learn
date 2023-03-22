; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

; 设置屏幕模式为文本模式， 清除屏幕
mov ax, 3
int 0x10

mov edi, 0x1000
mov ecx, 1
mov bl, 1
call read_disk

jmp 0x0000:0x1000

.halt:
    jmp .halt

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

    ; 3. 设置模式为LBA模式, 从主盘读
    mov dx, 0x1f6
    mov al, 0b1110_0000
    or al, cl ; 起始扇区位置最后高四位
    out dx, al

    ; 4. 设置为读硬盘
    mov dx, 0x1f7
    mov al, 0x20
    out dx, al


    xor ecx, ecx
    mov cl, bl

;; 对第14节的改进：每次read一个扇区后，需要检查一次硬盘是否已经读完。而不是像14节读了n个扇区后才开始检查一次。
.read:
    push cx
    call .waits
    call .reads
    pop cx
    loop .read
    popad
    ret

.waits:
    mov dx, 0x1f7
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
    ret


.reads:
    mov dx, 0x1f0
    mov cx, 256

    .readw:
        nop
        nop
        nop

        in ax, dx
        mov [edi], ax
        add edi, 2
        loop .readw
    ret


; 字符填充
times 510-($-$$) db 0x55

; 主引导记录结束标记
db 0x55, 0xaa