[bits 32] ; 指令集切换

extern kernel_init

global _start ; 全局符号定义, 链接器 ld 会将_start 标记为程序执行的起点
_start:
    mov byte [0xb8000], 'K'; 表示进入了内核

    ; 设置参数为msg
msg:
    db "Hello kernel!", 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    mov eax, msg             ; 设置参数为 msg 的地址
    push eax                 ; 压入参数
    call cdecl_call_print    ; 调用 print 函数
    add esp, 4               ; 清理栈（弹出 msg 的地址）
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;call kernel_init
    jmp $; 阻塞

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; *cdecl c语言调用约定。参数由调用者清理: add esp, n (n 表示参数占用的字节数) 由于调用者清理参数，因此支持可变参数；
; stdcall 标准调用约定 参数由函数清理: ret n (n 表示参数占用的字节数) 
;
; fastcall 使用寄存器传递前几个参数
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

cdecl_call_print:
    ; 设置栈帧
    ; 高地址
    ; +-----------------+
    ; | arg n           |
    ; | arg n-1         |
    ; | ...             |
    ; | arg 1           | ← EBP + 8（第一个参数） 1. 调用前设置函数参数
    ; | 返回地址         | ← EBP + 4             2. call命令压入eip
    ; | 保存的 EBP       | ← EBP（基址指针）       3. 保存调用的ebp
    ; | local variable  | ← EBP - 4             4. 局部变量
    ; | local variable  | ← EBP - 8
    ; | ...             |
    ; | local variable  | ← ESP（栈指针）
    ; +-----------------+
    ; 低地址
    ; 将8个基础寄存器压入栈中
    push ebp           ; 保存调用者基址指针
    mov ebp, esp       ; 设置新的基址指针（基址指针用于访问参数和局部变量）
    sub esp, 4         ; 分配一个局部变量存储显存地址 char *vdi;

    mov dword [ebp - 4], 0xb8000      ; 设置显存地址 vdi = 0xb8000;
.next:
    mov eax, [ebp + 8]    ; 加载字符串指针（参数）
    mov al, [eax]         ; 读取当前字符
    cmp al, 0             ; 检查是否为字符串结束
    jz .done

    mov edi, [ebp - 4]    ; 加载 vdi（显存地址）
    mov [es:edi], al      ; 写入字符到 VGA 缓冲区

    add edi, 2            ; vdi += 2（下一个字符位置）
    mov [ebp - 4], edi    ; 更新 vdi

    mov eax, [ebp + 8]    ; 加载字符串指针
    inc eax               ; c++（指向下一个字符）
    mov [ebp + 8], eax    ; 更新字符串指针

    jmp .next             ; 继续循环

.done:
    mov eax, 0            ; 设置返回值为 0
    mov esp, ebp          ; 恢复 ESP，释放局部变量
    pop ebp               ; 恢复调用者的 EBP
    ret                   ; 返回
