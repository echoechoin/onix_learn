# ((43.65 / 22) + (76.34 * 3.1)) / ((12.43 * 6) - (140.2 / 94.21))




.text

.globl main
main:
        pushl %ebp
        movl %esp, %ebp

# (43.65 / 22)
        finit             # 初始化浮点控制寄存器
        movl $0, %edi
        flds .value(%edi) # 将43.65中的值压入浮点寄存器栈
                          # st(0) = 43.65
                          # st(1) = 0.0
                          # ... ...
                          # st(7) = 0.0

        addl $4, %edi
        fidivs .value(%edi)  # 除以22
                             # st(0) = 43.65 / 22 ≈ 1.99
                             # st(1) = 0.0
                             # ... ...

        fstps .output1    # 将st(0)中的值弹出到内存中
                          # st(0) = 0.0
                          # st(1) = 0.0
                          # ... ...
        
        # 输出1
        # 将.output地址中的值压入栈中
        pushl .output1
        pushl .output2
        pushl $.fmt1
        call printf
        addl $8, %esp



        mov $0, %eax
        leave
        ret


.data
.value:
        .float 43.65
        .long     22
        .float 76.34
        .float   3.1
        .float 12.43
        .long      6
        .float 140.2
        .float 94.21

.output1:
        .long 0
.output2:
        .long 0


.fmt1:
        .asciz "The result is %.10f\n"


