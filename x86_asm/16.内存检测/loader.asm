[org 0x1000]

; xchg bx, bx
call check_memory
jmp halt

check_memory:
    mov ax, 0
    mov es, ax
    mov di, ards_buffer   ; es:di将获取到的内存信息写入此寄存器指向的内存

    xor ebx, ebx          ; 第一次调用时一定要置为0
    mov edx, 0x534D4150   ; 'SMAP'，用于标识内存信息

.next:
    mov eax, 0xe820      ; 0x15中断的子功能编号
    mov ecx, 20           ; ARDS 结构的字节大小 20字节
    int 0x15

    jc .error             ; CF=1表示出错

    add di, cx            ; 下一个存储内存信息的位置 di = di + 20。每个ARDS结构占20字节
    inc word [ards_count] ; ards_count++

    cmp ebx, 0            ; 等于零，说明内存检测结束
    jnz .next             ; 不等于零，说明还有内存信息，继续检测

    ; xchg bx, bx           ; 断点

    mov cx, [ards_count]  ; 获取内存信息结构体的数量
    mov si, 0

; ; 在寄存器中显示信息
; .show:
;     mov eax, [ards_buffer + si]      ; 获取起始地址
;     mov ebx, [ards_buffer + si + 8]  ; 获取内存长度
;     mov edx, [ards_buffer + si + 16] ; 获取内存类型
;     add si, 20                       ; 指向下一个ARDS结构
;     xchg bx, bx
;     loop .show                       ; 循环ards_count次

.error:

halt:
    jmp halt

message:
    dd 0xdeadbeef

; 地址范围描述符结构体大小
ards_count:
    dw 0

; 地址范围描述符结构体数组， 需要留出足够的空间！
ards_buffer:

