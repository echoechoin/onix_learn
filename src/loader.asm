[org 0x1000]

dw 0x55aa

; 使用BIOS中断打印启动日志
mov si, booting             ; si 寄存器通常用于字符串操作
call print

detect_memory:
    ; 1. 将 ebx 置为 0
    xor ebx, ebx

    ; 2. es:di 结构体的缓存位置: 指定BIOS将ards写到这里
    mov ax, 0
    mov es, ax
    mov edi, ards_buffer

    ; 3. 固定为签名标记 `0x534d4150`，此十六进制数字是字符串 `SMAP` 的ASCII 码
    mov edx, 0x534d4150; 固定签名

.next:
    ; 子功能号
    mov eax, 0xe820
    ; ards 结构的大小 (字节)
    mov ecx, 20
    ; 4. 调用 0x15 系统调用
    int 0x15

    ; CF：0 success 1 failed
    jc error

    ; 将缓存指针指向下一个结构体
    add di, cx

    ; 将结构体数量加一
    inc word [ards_count]

    ; 0表示最后一个ards
    cmp ebx, 0
    jnz .next

; 完成内存检测
.success:
    mov si, detecting
    call print

    ; 结构体数量
    mov cx, [ards_count]
    ; 结构体指针
    mov si, 0

.show:
    mov eax, [ards_buffer + si]
    mov ebx, [ards_buffer + si + 8]
    mov edx, [ards_buffer + si + 16]
    add si, 20
    loop .show
jmp $

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

error:
    mov si, .msg
    call print
    hlt; 让 CPU 停止
    jmp $
    .msg db "Loading Error!!!", 10, 13, 0

booting:
    db "Loading OS...", 10, 13, 0 ; \n\r
detecting:
    db "Detecting Memory Success...", 10, 13, 0; \n\r

ards_count:
    dw 0
ards_buffer:
