#include "os/os.h"
#include "os/console.h"
#include "os/printk.h"

char message[] = "hello onix!!!\n";
char buf[1024];

void kernel_init()
{
    console_init();
    while (true) {
        printk("hello world!\n");
    }

    return;
}
