CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0

objs = for.s while.s do_while.s if.s control.s
all: $(objs)

%.s: %.c
	gcc $(CFLAGS) -S $< -o $@

PHONY:clean
clean: 
	rm -f $(objs)

