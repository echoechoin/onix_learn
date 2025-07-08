#include "os/os.h"

int magic = OS_MAGIC;
char message[] = "hello onix!!!"; // .data
char buf[1024];                   // .bss

extern void kernel_init()
{
    char *video = (char *) 0xb8000; // 文本显示器的内存位置
    video[2] = message[0];
    for (int i = 0; i < sizeof(message); i++)
    {
        video[i * 2] = message[i];
    }
}
