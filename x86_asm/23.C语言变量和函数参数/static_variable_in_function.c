int func1(){
    static int a = 3;
}

int func2(){
    static int a = 5;
}

int main()
{
    static int a = 0;
    int b = 5;
    return 0;
}