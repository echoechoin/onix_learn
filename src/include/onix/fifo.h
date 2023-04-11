#ifndef ONIX_FIFO_H
#define ONIX_FIFO_H

#include "onix/types.h"

typedef struct fifo_t
{
    char *buf;
    uint32 length;
    uint32 head;
    uint32 tail;
} fifo_t;

/**
 * +---+---+---+---+---+---+---+---+---+---+
 * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
 * +---+---+---+---+---+---+---+---+---+---+
 *   ↑                   ↑
 *  tail                head
*/

// 将缓冲区初始化为一个空的FIFO
void fifo_init(fifo_t *fifo, char *buf, uint32 length);

// 判断FIFO是否已满
bool fifo_full(fifo_t *fifo);
bool fifo_empty(fifo_t *fifo);
char fifo_get(fifo_t *fifo);
void fifo_put(fifo_t *fifo, char byte);

#endif