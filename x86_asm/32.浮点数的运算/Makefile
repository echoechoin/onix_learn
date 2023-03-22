CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0 \
	 -g

all: float float_c

float:float.s
	gcc -m32 -o $@ $^ -static $(CFLAGS)

float_c:float_c.s
	gcc -m32 -o $@ $^ -static $(CFLAGS)

float_c.s:float_c.c
	gcc -m32 -S -o $@ $^ $(CFLAGS)

PHONY:clean
clean:
	rm -f float float_c float_c.s