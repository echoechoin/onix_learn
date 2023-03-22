#include <stdio.h>

extern int add(int a, int b);
extern int __attribute__((fastcall)) add_fast(int a, int b); // 表示此函数的调用约定为fastcall

int main()
{
    int a = 1;
    int b = 2;
    int c = add(1,2);
    printf("%d + %d = %d\n", a, b, c);

    int d =  add_fast(1,2);
    printf("%d + %d = %d\n", a, b, d);
    return 0;
}
