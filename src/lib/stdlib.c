#include <os/stdlib.h>

void delay(uint32_t count)
{
    while (count--)
        ;
}

void hang()
{
    while (true)
        ;
}
