CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0 \
	 -g

bcd: bcd.s
	gcc -m32 -o $@ $^ -static $(CFLAGS)
PHONY:clean
clean:
	rm -f bcd