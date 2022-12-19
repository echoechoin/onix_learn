[org 0x1000]

dw 0x55aa ; 用于判断错误

mov si, loading
call print

call detect_memory

jmp prepare_protected_mode


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

loading:
        db "Loading Onix...", 10, 13, 0 ; \n\r\0

detect_memory:
        pushad
        mov ax, 0
        mov es, ax
        mov di, ards_buffer   ; es:di将获取到的内存信息写入此寄存器指向的内存

        xor ebx, ebx          ; 第一次调用时一定要置为0, 之后这里会存储下一个内存信息的位置信息，由BIOS自动填充，我们不用管
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

        popad
        ret
;; 在寄存器中显示内存信息。
; .show:
;     mov eax, [ards_buffer + si]      ; 获取起始地址
;     mov ebx, [ards_buffer + si + 8]  ; 获取内存长度
;     mov edx, [ards_buffer + si + 16] ; 获取内存类型
;     add si, 20                       ; 指向下一个ARDS结构
;     xchg bx, bx
;     loop .show                       ; 循环ards_count次

.error:
        mov si, .msg
        call print
        hlt
        jmp $
        .msg db "Memory check error!", 0x0d, 0x0a, 0

prepare_protected_mode:

        ; 关闭中断
        cli

        ; 打开A20地址线
        in al, 0x92
        or al, 0b0000_0010
        out 0x92, al

        ; 加载GDT
        lgdt [gdt_ptr]

        ; 启动保护模式
        mov eax, cr0
        or eax, 0x1
        mov cr0, eax

        ; 用跳转刷新指令缓冲区
        jmp dword code_selector:protected_mode

[bits 32]
protected_mode:
        mov ax, data_selector
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        mov esp, 0x10000

        mov byte [0xb8000], 'P'

        mov edi, 0x10000 ; 目标地址
        mov ecx, 10      ; 起始扇区
        mov bl, 200      ; 读取扇区数
        call read_disk
        jmp dword code_selector:0x10000

        ud2; 表示出错

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

code_selector equ (1 << 3)
data_selector equ (2 << 3)

; 段基地址
memory_base equ 0
; 段界限（4KB为单位，因此需要除以4KB再减一）
memory_limit equ (1024 * 1024 * 1024 * 4) / (4 * 1024) - 1

gdt_ptr:
        dw gdt_end - gdt_base - 1
        dd gdt_base
gdt_base:
        dd 0, 0      ; GDT NULL 描述符
gdt_code:
    ; 段界限15-0
    dw memory_limit & 0xffff
    ; 段基地址15-0
    dw memory_base & 0xffff
    ; 段基地址23-16
    db (memory_base >> 16) & 0xff
    ; 代码段，依从，可读，未访问 | 代码段或者数据段 | 特权级0 | 在内存中
    db 0b0000_1110 | 0b0001_0000 | 0b0000_0000 | 0b1000_0000
    ; 4KB | 32位 | 非64位 | 没用 | 段界限19-16
    db 0b1000_0000 | 0b0100_0000 | 0b0000_0000 | (memory_limit >> 16)
    ; 段基地址31-24
    db (memory_base >> 24) & 0xff

gdt_data: ; 数据段
    ; 段界限15-0
    dw memory_limit & 0xffff
    ; 段基地址15-0
    dw memory_base & 0xffff
    ; 段基地址23-16
    db (memory_base >> 16) & 0xff
    ; 数据段，栈向下扩展，可写，未访问 | 代码段或者数据段 | 特权级0 | 在内存中
    db 0b0000_0010 | 0b0001_0000 | 0b0000_0000 | 0b1000_0000
    ; 4KB | 32位 | 非64位 | 没用 | 段界限19-16
    db 0b1000_0000 | 0b0100_0000 | 0b0000_0000 | (memory_limit >> 16)
    ; 段基地址31-24
    db (memory_base >> 24) & 0xff
gdt_end:


; 地址范围描述符结构体大小
ards_count:
        dw 0

; 地址范围描述符结构体数组， 需要留出足够的空间！
ards_buffer:
        times 1000 db 0

