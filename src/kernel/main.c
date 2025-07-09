#include "os/os.h"
#include "os/console.h"
#include "os/printk.h"
#include "os/assert.h"
#include "os/global.h"
#include "os/task.h"

char message[] = "hello onix!!!\n";
char buf[1024];

void kernel_init()
{
    console_init();
    gdt_init();
    show_gdt_list();
    task_init();

    return;
}
