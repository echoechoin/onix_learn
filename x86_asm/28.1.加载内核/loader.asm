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
    ; mov word [gs:0x2000], 0x55aa ; 访问test段的0x2000处，触发异

    call setup_page

    call load_kernel
    
    xchg bx, bx

    jmp code_selector:0xc0011000


halt:
    jmp halt

; 页目录存放位置 需要占用一页的内存，后12位必须为0(4k对齐)
PDE equ 0x2000
; 一页的页表存放位置
PTE equ 0x3000
; 页属性
PAGE_ATTR equ 0b11 ; 存在，可写

setup_page:
    ; 清空PDE和PTE这两个页
    mov eax, PDE
    call .clear_page
    mov eax, PTE
    call .clear_page

    ; 将前面的1M内存映射到 0x00000000 ~ 0x00100000
    ; 将前面的1M内存映射到 0xC0000000 ~ 0xC0100000

    mov eax, PTE ; 设置页表的地址：0x3000
    or eax, PAGE_ATTR ; 设置页表属性
    mov [PDE + 0x000 * 4], eax ; 设置PDE[0x0000]为PTE的地址，这样就可以访问0x00000000 ~ 0x00100000了
                               ;         0b0000_0000_00__00_0000_0000__0000_0000_0000_0000
                               ;         PDE(10位)        PTE(10位)     offset（12位）
                               ; 前十位等于0x000 是页目录的第0项索引

    mov [PDE + 0x300 * 4], eax ; 设置页目录PDE[0x300]为PTE的地址，这样就可以访问0xC0100000 ~ 0xC0200000了
                               ;         0b1100_0000_00__00_0000_0000__0000_0000_0000_0000
                               ;         PDE(10位)        PTE(10位)     offset（12位）
                               ; 前十位等于0x300 是页目录的第0x300项索引
    mov eax, PDE
    or eax, PAGE_ATTR
    mov [PDE + 0x3ff * 4], eax ; 把最后一个索引指向自己，这样就可以访问自己了
                               ;         0b1111_1111_11__00_0000_0000__0000_0000_0000_0000
                               ;         PDE(10位)        PTE(10位)     offset（12位）
                               ; 前十位等于0x3ff 是页目录的第0x3ff项索引


    mov ebx, PTE
    mov ecx, (0x100000 / 0x1000) ; 1M / 4K = 256 个页表项
    mov esi, 0 ; 页表项的索引，也是物理地址的高20位

; 填充256个页表项
.next_page:
    mov eax, esi
    shl eax, 12
    or eax, PAGE_ATTR ; 设置页表属性

    mov [ebx + esi * 4], eax ; 设置页
    inc esi
    loop .next_page

    mov eax, PDE
    mov cr3, eax ; 加载页目录

    mov eax, cr0
    or eax, 0x8000_0000
    mov cr0, eax ; 开启分页

    ret

.clear_page:
    ;清空一个内存页， 地址参数存在eax中
    mov ecx, 0x1000 ; 表示一个页的大小
    mov esi, 0

.set:
    mov byte [eax + esi], 0
    inc esi
    loop .set
    ret



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

load_kernel:
    mov edi, 0x10000
    mov ecx, 10
    mov bl, 100
    call read_disk
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

    ; 3. 设置模式为LBA模式, 从主盘读
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
    mov cx, ax ; 循环次数， 每次读取256个word

    .readw:
        nop
        nop
        nop

        in ax, dx
        mov [edi], ax
        add edi, 2
        loop .readw
    popad
    ret
