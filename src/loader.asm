[org 0x1000]

dw 0x55aa

; 使用BIOS中断打印启动日志
mov si, booting             ; si 寄存器通常用于字符串操作
call print

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

; 存放booting字符串
booting:
    db "Loading OS...", 10, 13, 0 ; \n\r
