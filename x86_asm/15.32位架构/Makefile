NASM = nasm
GCC = gcc
BXIMAGE = bximage
BOCHS = bochs
DISK_IMG = ../build/master.img
BOCHSRC = ../build/bochsrc

# 编不过，因为是64位的操作系统，找不到32位的库
hello: hello.o
	$(GCC) -m32 -o hello hello.o -static

hello.o: hello.asm
	$(NASM) -f elf32 -o $@ $<