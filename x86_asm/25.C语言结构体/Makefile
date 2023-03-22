CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0

# CFLAGS += -fverbose-asm

objs = structure.s bit.s union.s enum.s
all: $(objs)

%.s: %.c
	gcc $(CFLAGS) -S $< -o $@

PHONY:clean
clean: 
	rm -f $(objs)

