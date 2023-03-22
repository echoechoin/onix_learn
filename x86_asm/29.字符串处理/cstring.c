
#include <stdio.h>

typedef unsigned int size_t;

void *memcpy_s(void *dest, const void *src, size_t count);
void *memset_s(void *dest, int ch, size_t count);
int memcmp_s(const void *lhs, const void *rhs, size_t count);
void *memchr_s(const void *str, int c, size_t n);

void *memcpy(void *dest, const void *src, size_t count)
{
    char *dptr = (char *)dest;
    const char *sptr = (const char *)src;
    for (size_t i = 0; i < count; i++)
    {
        dptr[i] = sptr[i];
    }
    return dest; 
}

char message[] = "hello world";
char buffer1[sizeof(message)] = {1};
char buffer2[sizeof(message)];

int main()
{
    memset_s(buffer1, 0, sizeof(buffer1));
    printf("buffer1: %d\n", buffer1[0]);
    
    memcpy(buffer1, message, sizeof(message));
    memcpy_s(buffer2, message, sizeof(message));
    printf("buffer1: %s\n", buffer1);
    printf("buffer2: %s\n", buffer2);

    buffer2[5] = '/';
    int ret = memcmp_s(buffer1, buffer2, sizeof(buffer1));
    printf("ret: %d\n", ret);

    char *p = memchr_s(buffer1, 'w', sizeof(buffer1));
    if (p)
    {
        printf("p: %p, c: %c\n", p, *p);
    }
    else
    {
        printf("not found\n");
    }

    return 0;
}