
.text

.globl add
add:
    pushl %ebp
    # 当前栈帧内容
    # ebp   <- esp
    # eip
    # a
    # b
    movl %esp, %ebp
 
    movl 8(%ebp), %eax
    movl 12(%ebp), %ebx
    addl %ebx, %eax

    # 调用printf函数
    pushl %eax
    pushl 12(%ebp)
    pushl 8(%ebp)
    pushl $.msg
    call printf
    addl $12, %esp
    popl %eax

    leave
    ret

.data

.msg:
    .asciz "%d + %d = %d\n"
