#ifndef _ONIX_KEYBOARD_H_
#define _ONIX_KEYBOARD_H_

#include "onix/interrupt.h"
#include "onix/io.h"
#include "onix/assert.h"
#include "onix/debug.h"
#include "onix/fifo.h"
#include "onix/mutex.h"
#include "onix/task.h"

void keyboard_init();
uint32 keyboard_read(char *buf, uint32 count);

#endif
