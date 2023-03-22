#ifndef ONIX_STDARG_H
#define ONIX_STDARG_H

typedef char * va_list;

// va_start()将ap指向第一个可变参数。
#define va_start(ap, v) (ap = (va_list)&v + sizeof(v))

// va_arg()返回ap指向的参数，并将ap指向下一个参数。
#define va_arg(ap, t) (*(t *)((ap += sizeof(t)) - sizeof(t)))

// va_end()将ap置为0。
#define va_end(ap) (ap = (va_list)0)

#endif