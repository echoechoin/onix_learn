CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -O0

objs= variable.s \
    local_variable.s \
    static_variable_in_function.s \
    stack_protect.s stack_protect \
    no_stack_protect \
    parameter_passing.s

all: $(objs)

variable.s:variable.c
	gcc -S -o $@ $< $(CFLAGS)

local_variable.s:local_variable.c
	gcc -S -o $@ $< $(CFLAGS)

static_variable_in_function.s:static_variable_in_function.c
	gcc -S -o $@ $< $(CFLAGS)

stack_protect:stack_protect.c
	gcc -o $@ $<

no_stack_protect:stack_protect.c
	gcc -o $@ -fno-stack-protector $<

stack_protect.s:stack_protect.c
	gcc -S -o $@ $< $(CFLAGS)

no_stack_protect.s:stack_protect.c
	gcc -S -o $@ $< $(CFLAGS) -fno-stack-protector

parameter_passing.s:parameter_passing.c
	gcc -S -o $@ $< $(CFLAGS)

PHONY:clean
clean: 
	rm -f $(objs)

