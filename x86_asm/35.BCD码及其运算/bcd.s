.data

value1: .long 0x05060708 # 5678
value2: .long 0x01020304 # 1234
value3: .long 0          # value1 + value2

value4: .long 0x05060708 # 5678
value5: .long 0x03090809 # 3989
value6: .long 0          # value4 - value5


fmt: .asciz "value3 = 0x%x\n"
.text

.globl main
main:
        pushl %ebp
        movl %esp, %ebp

#进位加法

        movl $4, %ecx # 循环4次
        xor %edi, %edi # edi = 0
loop1:
        movb value1(%edi), %al # al = value1[edi] = 8
        adc value2(%edi), %al # al = al + value2[edi] + CF = 8 + 4 + 0 = 12 
        aaa # 调整成了：al = 2 CF = 1

        movb %al, value3(%edi) # value3[edi] = al = 2
        inc %edi
        loop loop1

        push value3
        push $fmt
        call printf # 0x06090102
        addl $8, %esp

# 借位减法

        movl $4, %ecx # 循环4次
        xor %edi, %edi # edi = 0
loop2:
        movb value4(%edi), %al # al = value4[edi] = 8
        sbb value5(%edi), %al # al = al - value5[edi] - AF = 0008 - 0009 - 0 = 0xFF09 TODO
        aas # 调整成了：al = 0x0009 AF = 1

        movb %al, value6(%edi) # value3[edi] = al = 2
        inc %edi
        loop loop2

        push value6
        push $fmt
        call printf # 0x06090102
        addl $8, %esp
        


        leave
        ret

