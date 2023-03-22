typedef struct 
{
    char c1:4;
    char c2:2;
    char c3:1;
    char c4:1;
}struct_t;

struct_t data;

int main()
{
    data.c1 = 0b1010;
    data.c2 = 0b10;
    data.c3 = 0b1;
    data.c4 = 0b0;
}