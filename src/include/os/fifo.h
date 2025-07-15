#ifndef OS_FIFO_H
#define OS_FIFO_H

#include <os/types.h>

typedef struct fifo_t
{
    char *buf;
    uint32_t length;
    uint32_t head;
    uint32_t tail;
} fifo_t;

void fifo_init(fifo_t *fifo, char *buf, uint32_t length);
bool fifo_full(fifo_t *fifo);
bool fifo_empty(fifo_t *fifo);
char fifo_get(fifo_t *fifo);
void fifo_put(fifo_t *fifo, char byte);

#endif