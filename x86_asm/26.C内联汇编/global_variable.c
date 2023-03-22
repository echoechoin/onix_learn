#include <stdio.h>

int a = 10;
int b = 20;
int result = 0;

int main()
{
    // result = a + b;
    asm("movl a, %eax");
    asm("movl b, %ebx");
    asm("addl %ebx, %eax");
    asm("movl %eax, result");
    printf("result = %d\n", result);
}