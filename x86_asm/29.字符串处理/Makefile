CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0 \
	 -g

string:cstring.o astring.o
	gcc -m32 -o $@ $^ -static

astring.o:astring.s
	gcc -m32 -c $^ -o $@ $(CFLAGS)

cstring.o:cstring.s
	gcc -m32 -c $^ -o $@ $(CFLAGS)

cstring.s:cstring.c
	gcc -m32 -S $^ -o $@ $(CFLAGS)

PHONY:clean
clean:
	rm -f cstring.o astring.o string cstring.s
