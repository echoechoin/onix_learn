typedef struct 
{
    char c1;
    int i1;
}struct_t;

typedef union
{
    char value;
    struct_t m;
}union_t;


union_t udata;

int main()
{
    udata.value = 0b10101010;
    udata.m.c1 = 0x12;
    udata.m.i1 = 0x12345678;
}