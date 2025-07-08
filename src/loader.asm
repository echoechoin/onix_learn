[org 0x1000]

dw 0x55aa

; 使用BIOS中断打印启动日志
mov si, loading             ; si 寄存器通常用于字符串操作
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

prepare_protected_mode:
    cli; 关闭中断

    ; 打开 A20 线
    in al,  0x92
    or al, 0b10
    out 0x92, al

    lgdt [gdt_ptr]; 加载 gdt

    ; 启动保护模式
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; 用跳转来刷新缓存，启用保护模式
    jmp dword code_selector:protect_mode

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

loading:
    db "Loading OS...", 10, 13, 0 ; \n\r
detecting:
    db "Detecting Memory Success...", 10, 13, 0; \n\r


[bits 32]
protect_mode:
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax; 初始化段寄存器

    mov esp, 0x10000; 修改栈顶
    mov byte [0xb8000], 'P' ; VGA文本模式
    mov byte [0xb8001], 0xd ; 粉红色

jmp $;

code_selector equ (1 << 3)
data_selector equ (2 << 3)

memory_base equ 0; 内存开始的位置：基地址

; 内存界限 4G / 4K - 1
memory_limit equ ((1024 * 1024 * 1024 * 4) / (1024 * 4)) - 1

gdt_ptr:
    dw (gdt_end - gdt_base) - 1 ; 大小,（以字节为单位，减 1 符合 x86 规范）。
    dd gdt_base ; 基地址
gdt_base:
    dd 0, 0; NULL 描述符 GDT 的第一个描述符必须是 NULL 描述符，全为 0，用作占位符。
gdt_code:
    dw memory_limit & 0xffff; 段界限 0 ~ 15 位
    dw memory_base & 0xffff; // 基地址 0 ~ 16 位
    db (memory_base >> 16) & 0xff; // 基地址 0 ~ 16 位
    ; 存在 - dlp 0 - S _ 代码 - 非依从 - 可读 - 没有被访问过
    db 0b_1_00_1_1_0_1_0;
    ; 4k - 32 位 - 不是 64 位 - 段界限 16 ~ 19
    db 0b1_1_0_0_0000 | (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff; 基地址 24 ~ 31 位
gdt_data:
    dw memory_limit & 0xffff; 段界限 0 ~ 15 位
    dw memory_base & 0xffff; // 基地址 0 ~ 16 位
    db (memory_base >> 16) & 0xff; // 基地址 0 ~ 16 位
    ; 存在 - dlp 0 - S _ 数据 - 向上 - 可写 - 没有被访问过
    db 0b_1_00_1_0_0_1_0;
    ; 4k - 32 位 - 不是 64 位 - 段界限 16 ~ 19
    db 0b1_1_0_0_0000 | (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff; 基地址 24 ~ 31 位
gdt_end:

ards_count:
    dw 0
ards_buffer:
