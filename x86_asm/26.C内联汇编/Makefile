CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0

# CFLAGS += -fverbose-asm

objs = assembly.s assembly global_variable.s global_variable \
       local_variable.s local_variable
all: $(objs)

assembly: assembly.s
	gcc -m32 -o $@ $< -static

assembly.s: assembly.c
	gcc $(CFLAGS) -S $< -o $@

global_variable: global_variable.s
	gcc -m32 -o $@ $< -static

global_variable.s: global_variable.c
	gcc $(CFLAGS) -S $< -o $@

local_variable: local_variable.s
	gcc -m32 -o $@ $< -static

local_variable.s: local_variable.c
	gcc $(CFLAGS) -S $< -o $@

PHONY:clean
clean: 
	rm -f $(objs)

