hello:hello.o
	@# gcc 会自动加载一个start文件，这个文件会调用main函数，且main函数结束后会调用exit函数
	@# gcc -o hello hello.o -static
	@# 不需要gcc加载startfile，指定_start为main函数入口， 
	@#     此时如果不调用exit函数，程序会一直运行，
	@#     如果访问了未分配的内存，会导致程序崩溃（段错误）
	gcc -o hello hello.o -static -nostartfiles -e main

hello.o:hello.s
	as -o hello.o hello.s

PHONY:show_elf
show_elf:
	readelf -e hello.o

PHONY:show_intel_asm
show_intel_asm:
	objdump -d hello.o -M intel

PHONY:show_att_asm
show_att_asm:
	objdump -d hello.o -M att

PHONY:clean
clean: 
	rm -f hello hello.o 