.section .text

.globl main
main:
    movl $4, %eax               # 4 is the syscall number for write
    movl $1, %ebx               # 1 is the file descriptor for stdout
    movl $msg, %ecx             # the address of the string to output
    movl $(msg_end - msg), %edx # the length of the string
    # int $0x80                   # call the kernel


    # 寻址方式演示：
    movb $'A', msg              # 将'H'改为'A' /                               msg[0] = 'A'
    movb $'B', 1(%ecx)          # 将'A'改为'B' / 1 + %ecx = msg + 1            msg[2] = 'B'
    movb $'C', 1(%ecx, %ebx, 2) # 将'l'改为'C' / 1 + %ecx + %ebx * 2 = msg + 4 msg[4] = 'C'
    movb $'D', msg(, %ebx, 4)   # 将'o'改为'D' / msg + 0 + %ebx * 4 = msg + 5  msg[5] = 'D'
    int $0x80                   # call the kernel

    movl $1, %eax               # 1 is the syscall number for exit
    movl $0, %ebx               # 0 is the exit code
    int $0x80                   # call the kernel

.section .data
msg:
    .asciz "Hello, world!\n"    # 以0结尾的字符串
msg_end:
