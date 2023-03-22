CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0 \
	 -g

call:call.s subcall.s fastcall.s
	gcc -m32 -o $@ $^ -static $(CFLAGS)

call.s:call.c
	gcc -m32 -S $< -o $@ $(CFLAGS)

PHONY:clean
clean:
	rm -f call call.s
