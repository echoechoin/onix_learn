#ifndef ONIX_DEBUG_H
#define ONIX_DEBUG_H

void debugk(char *file, int line, char *fmt, ...);

#define BMB asm volatile("xchgw %bx, %bx");
// ##args表示将`args...`复制到`##args`处。
// 如果没有可变参数，`##args`将为空且预编译的代码将不包含`##args`前面的逗号。
#define DEBUGK(fmt, args...) debugk(__FILE__, __LINE__, fmt, ##args)

#endif