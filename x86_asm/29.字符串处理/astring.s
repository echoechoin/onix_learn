.text
.globl memcpy_s
memcpy_s:
    pushl %ebp
    movl %esp, %ebp
    # stack：
    # ebp
    # eip
    # dst
    # src
    # count
    movl 16(%ebp), %ecx # count
    movl 12(%ebp), %esi
    movl 8(%ebp), %edi

    cld # 清方向位DF:DF为0是正向拷贝（从低地址到高地址），为1是反向拷贝（从高地址到低地址）

    rep movsb # ss:[esi] -> ds:[edi]，并且esi和edi都加1(DF为1时减1)
              # rep指令会重复执行movsb指令，直到ecx为0，每次重复执行都会将ecx减1
    
    leal 8(%ebp), %eax
    leave
    ret

.globl memset_s
memset_s:
    pushl %ebp
    movl %esp, %ebp
    # stack：
    # ebp
    # eip
    # dest
    # ch
    # count
    movl 16(%ebp), %ecx # count
    movl 12(%ebp), %eax # ch
    movl 8(%ebp),  %edi # dest

    cld # 清方向位DF:DF为0是正向

    rep stosl # ds:[edi] <- eax，并且edi都加4(DF为1时减4)
              # rep指令会重复执行stosd指令，直到ecx为0，每次重复执行都会将ecx减1
    
    leal 8(%ebp), %eax
    leave
    ret

.globl memcmp_s
memcmp_s:
    pushl %ebp
    movl %esp, %ebp
    # stack：
    # ebp
    # eip
    # lhs
    # rhs
    # count
    movl 16(%ebp), %ecx # count
    movl 12(%ebp), %esi
    movl 8(%ebp), %edi

    cld # 清方向位DF:DF为0是正向拷贝（从低地址到高地址），为1是反向拷贝（从高地址到低地址）

    repe cmpsb # ss:[esi] -> ds:[edi]，并且esi和edi都加1(DF为1时减1)
               # repe指令会重复执行cmpsb指令，直到ecx为0或者两个字节不相等，每次重复执行都会将ecx减1
               # 如果两个字节不相等，ZF为0，此时也会导致repe指令结束。否则ZF为1，repe指令继续执行


    subl %ecx, 16(%ebp)
    movl 16(%ebp), %eax
    leave
    ret

.globl memchr_s
memchr_s:
    pushl %ebp
    movl %esp, %ebp
    # stack：
    # ebp
    # eip
    # str
    # c
    # n
    movl 16(%ebp), %ecx # n
    movb 12(%ebp), %al  # c
    movl 8(%ebp), %edi  # str

    cld # 清方向位DF:DF为0是正向拷贝（从低地址到高地址），为1是反向拷贝（从高地址到低地址）

    repne scasb # ds:[edi] 是否等于al，如果等于edi加1(DF为1时减1)
               # repe指令会重复执行scanb指令，直到ecx为0或者两个字节不相等，每次重复执行都会将ecx减1
               # 如果两个字节不相等，ZF为0，此时也会导致repe指令结束。否则ZF为1，repe指令继续执行

    mov $0, %eax
    subl $1, %edi
    cmpl $0, %ecx  # 判断ecx是否为0，如果为0，则表示没有找到，返回NULL
    cmovne %edi, %eax # 如果ecx不为0，则表示找到了，返回c的地址

    leave
    ret
    



