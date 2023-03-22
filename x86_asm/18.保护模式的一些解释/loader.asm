[org 0x1000]

; xchg bx, bx
jmp prepare_protect_mode

prepare_protect_mode:
    ; 1. 进入保护模式首先需要关闭中断
    cli

    ; 2. 打开A20地址线
    in al, 0x92
    or al, 0x02       ; 把0x92端口的第1位置1再写回去即可
    out 0x92, al

    ; 3. 加载gdt
    lgdt [gdt_ptr]

    ; 4. 打开保护模式
    mov eax, cr0 ; cr0寄存器无法直接写入，需要先读出来
    or eax, 0x1
    mov cr0, eax

    ; 5. 跳转到32位的代码中。这里就开始使用选择子了。除此之外还为了清空流水线
    jmp dword code_selector:prot_mode_start


[bits 32]
prot_mode_start:
    ; 初始化数据段寄存器
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    xchg bx, bx

    mov esp, 0x10000 ; 设置栈顶

    mov byte [0xb8000], 'H'
    mov byte [0xb8002], 'e'
    mov byte [0xb8004], 'l'
    mov byte [0xb8006], 'l'
    mov byte [0xb8008], 'o'
    mov byte [0xb800a], ' '
    mov byte [0xb800c], 'W'
    mov byte [0xb800e], 'o'
    mov byte [0xb8010], 'r'
    mov byte [0xb8012], 'l'
    mov byte [0xb8014], 'd'
    mov byte [0xb8016], '!' 

    mov word [gs:0], 0x55aa ; 访问test段的0x0处

    xchg bx, bx

    mov word [gs:0x2000], 0x55aa ; 访问test段的0x2000处，触发异常

halt:
    jmp halt

message:
    dd 0xdeadbeef

; 代码段选择子
code_selector equ (1 << 3) ; 最后的3位是RPL和ti，这里设置为0
; 数据段选择子
data_selector equ (2 << 3)


base equ 0
limit equ 0xfffff


; 全局描述符表
gdt_base:

gdt_frist: ; 第一个描述符是空的
    dd 0, 0

;代码段
gdt_code:
    ; 段界限15-0
    dw limit & 0xffff
    ; 段基地址15-0
    dw base & 0xffff
    ; 段基地址23-16
    db (base >> 16) & 0xff
    ; 代码段，依从，可读，未访问 | 代码段或者数据段 | 特权级0 | 在内存中
    db 0b0000_1110 | 0b0001_0000 | 0b0000_0000 | 0b1000_0000
    ; 4KB | 32位 | 非64位 | 没用 | 段界限19-16
    db 0b1000_0000 | 0b0100_0000 | 0b0000_0000 | (limit >> 16)
    ; 段基地址31-24
    db (base >> 24) & 0xff



gdt_data: ; 数据段
    ; 段界限15-0
    dw limit & 0xffff
    ; 段基地址15-0
    dw base & 0xffff
    ; 段基地址23-16
    db (base >> 16) & 0xff
    ; 数据段，栈向下扩展，可写，未访问 | 代码段或者数据段 | 特权级0 | 在内存中
    db 0b0000_0010 | 0b0001_0000 | 0b0000_0000 | 0b1000_0000
    ; 4KB | 32位 | 非64位 | 没用 | 段界限19-16
    db 0b1000_0000 | 0b0100_0000 | 0b0000_0000 | (limit >> 16)
    ; 段基地址31-24
    db (base >> 24) & 0xff

gdt_test:
    dw 0xfff
    dw 0x0000
    db 0x1
    db 0b1001_0010 ;0x92
    db 0b0100_0000 ; 0x40
    db 0x0

gdt_end:

gdt_ptr:
    dw gdt_end - gdt_base - 1
    dd gdt_base