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
        inc dword [ards_count] ; ards_count++

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

        ; 打开A20地址线，否者无法访问1M以上的内存
        in al, 0x92
        or al, 0b0000_0010
        out 0x92, al

        ; 加载GDT
        lgdt [gdt_ptr]

        ; 启动保护模式： 
        ; cr0寄存器的第0位是PE位，用来控制CPU的工作模式。置为1表示进入保护模式。
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

        ; 添加一个魔数
        mov eax, 0x12345678

        ; 保存 ards_count 的地址到ebx中
        mov ebx, ards_count

        ; 跳转到kernel中，由于有multiboot2头，所以这里需要跳转到0x10040位置的text段
        jmp dword code_selector:(0x10040)

        ud2; 表示出错

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

code_selector equ (1 << 3)
data_selector equ (2 << 3)

; 段基地址
memory_base equ 0
; 段界限（4KB为单位，因此需要除以4KB再减一）
memory_limit equ (1024 * 1024 * 1024 * 4) / (4 * 1024) - 1

gdt_ptr:
        dw gdt_end - gdt_base - 1
        dd gdt_base; GDT基地址
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
        dd 0

; 地址范围描述符结构体数组， 需要留出足够的空间！
ards_buffer:
        times 1000 db 0

