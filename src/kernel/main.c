#include "os/os.h"
#include "os/types.h"
#include "os/io.h"
#include "os/string.h"
#include "os/console.h"

char message[] = "hello onix!!!\n";
char buf[1024];

void kernel_init()
{
    console_init();
    while (true)
    {
        console_write(message, sizeof(message) - 1);
    }

    return;
}
