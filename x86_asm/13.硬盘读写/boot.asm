; 编译后的程序中的标签将以0x7c00为基地址，而不是以0为基地址
[org 0x7c00]

;;;; 将主引导扇区写到0x1000位置（因为0x1000可用，所以写在这里了）

; 1. 设置扇区数量为1个
mov dx, 0x1f2
mov al, 1
out dx, al

; 2. 设置从第0个扇区开始读
mov al, 0
mov dx, 0x1f3
out dx, al

mov dx, 0x1f4
out dx, al

mov dx, 0x1f5
out dx, al

; 3. 设置模式为LBA模式, 从主盘读
mov dx, 0x1f6
mov al, 0b1110_0000
out dx, al

; 4. 设置为读硬盘
mov dx, 0x1f7
mov al, 0x20
out dx, al

.check_read_state:
    ; ata要求检查状态前需要一点点的延迟
    nop
    nop
    nop
    ; 5. 判断硬盘是否准备好
    mov dx, 0x1f7
    in al, dx
    and al, 0b1000_1000 ; 第7位表示硬盘是否忙, 第3位表示硬盘是否准备好
    cmp al, 0b0000_1000 ; 检查是否可读
    jnz .check_read_state

; 6. 读取数据
mov ax, 0x100
mov es, ax
mov di, 0
mov dx, 0x1f0

.read_loop:
    nop
    nop
    nop
    in ax, dx
    mov [es:di], ax
    add di, 2
    cmp di, 512
    jnz .read_loop

;;;; 将读出来的数据写入到第二个扇区

; 1. 设置扇区数量为1个
mov dx, 0x1f2
mov al, 1
out dx, al

; 2. 设置从第1个扇区开始写
mov al, 1
mov dx, 0x1f3
out dx, al

mov al, 0
mov dx, 0x1f4
out dx, al

mov dx, 0x1f5
out dx, al

; 3. 设置模式为LBA模式, 从主盘写
mov dx, 0x1f6
mov al, 0b1110_0000
out dx, al

; 4. 设置为写硬盘
mov dx, 0x1f7
mov al, 0x30
out dx, al

; 5. 设置src的地址
mov ax, 0x100
mov es, ax
mov di, 0

; 6. 设置端口为读写硬盘
mov dx, 0x1f0

; 7. 写入数据
.write_loop:
    nop
    nop
    nop

    mov ax, [es:di]
    out dx, ax

    add di, 2
    cmp di, 512
    jnz .write_loop

; 8. 检测状态
mov dx, 0x1f7
.check_write_state:
    ; ata要求检查状态前需要一点点的延迟
    nop
    nop
    nop
    ; 5. 判断硬盘是否写完了
    in al, dx
    and al, 0b1000_0000 ; 第7位表示硬盘是否忙, 第3位表示硬盘是否准备好
    cmp al, 0b1000_0000 ; 检测是否为busy，如果是busy则继续检测
    jz .check_write_state

.halt:
    jmp .halt

; 字符填充
times 510-($-$$) db 0x55

; 主引导记录结束标记
db 0x55, 0xaa