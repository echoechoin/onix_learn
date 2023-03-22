
.text

.globl add_fast
add_fast:
    pushl %ebp
    # 当前栈帧内容
    # ebp   <- esp
    # eip
    # a
    # b
    movl %esp, %ebp

    movl %ecx, %eax
    addl %edx, %eax

    # 调用printf函数
    pushl %eax
    pushl %edx
    pushl %ecx
    pushl $.msg
    call printf
    addl $12, %esp
    popl %eax

    leave
    ret

.data

.msg:
    .asciz "%d + %d = %d\n"
