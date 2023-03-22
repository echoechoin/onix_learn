CFLAGS = -m32 \
	 -fno-asynchronous-unwind-tables \
	 -fno-pic \
	 -mpreferred-stack-boundary=2 \
	 -fno-builtin \
	 -fno-stack-protector \
	 -O0

compile:compile.o
	gcc $(CFLAGS) -o compile compile.o -static
	# 等同于：ld -m elf_i386 -static -o compile compile.o

compile.o: compile.s
	gcc $(CFLAGS) -c $< -o $@
	# 等同于：as -32 $< -o $@

compile.s:compile.i
	gcc $(CFLAGS) -S $< -o $@

compile.i:compile.cc
	gcc -E $< -o $@