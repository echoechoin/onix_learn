#include <stdio.h>

int main()
{
    // exit(0);
    asm("movl $1, %eax");
    asm("movl $0, %ebx");
    asm("int $0x80");
}