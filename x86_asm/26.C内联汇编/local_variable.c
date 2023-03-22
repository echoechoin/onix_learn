#include <stdio.h>


int main()
{
        
    int a = 10;
    int b = 20;
    int result = 0;

    // result = a + b;
    asm volatile(
        "addl %%edx, %%eax\n"
        : "=a"(result)     // output
        : "a"(a), "d"(b)   // input: 把a的值放到eax寄存器中，把b的值放到edx寄存器中
    );
    
    // 使用占位符
    asm volatile(
        "addl %1, %2\n"
        "movl %2, %0\n"
        : "=r"(result)     // output
        : "r"(a), "r"(b)   // input: 把a的值放到eax寄存器中，把b的值放到edx寄存器中
    );

    // 使用引用占位符
    // result += a;
    asm volatile(
        "addl %1, %2\n"
        "movl %2, %0\n"
        : "=r"(result)     // output
        : "r"(a), "0"(result)   // input: 把a的值放到eax寄存器中，把b的值放到edx寄存器中
    );

    // 使用命名占位符
    // result += a;
    asm volatile(
        "addl %[a], %[result]\n"
        "movl %[result], %[result]\n"
        : [result]"=r"(result)     // output
        : [a]"r"(a), "0"(result)   // input: 把a的值放到eax寄存器中，把b的值放到edx寄存器中
    );

    // 指定改动的寄存器列表
    // result = a + b;
    asm volatile(
        "movl %[a], %%eax\n"
        "movl %[b], %%edx\n"
        "addl %%edx, %%eax\n"
        "movl %%eax, %[result]\n"
        : [result]"=r"(result)     // output
        : [a]"r"(a), [b]"r"(b)   // input: 把a的值放到eax寄存器中，把b的值放到edx寄存器中
        : "eax", "edx"     // 改动的寄存器列表
    );

    // 使用内存位置
    asm volatile(
        "addl %[var1], %[var2]\n"
        "movl %[var1], %[result]\n"
        : [result] "=m"(result)     // output
        : [var1]   "m" (a), 
          [var2]   "m" (b)   // input: 把a的值放到eax寄存器中，把b的值放到edx寄存器中
    );


    printf("result = %d\n", result);
}