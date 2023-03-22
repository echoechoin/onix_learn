all: hello.s hello.asm

hello.s:hello.c
	@# -S 生成汇编代码
	@# -fno-asynchronous-unwind-tables 不生成调用栈信息
	@# -fno-pic 不生成位置无关代码
	@# -mpreferred-stack-boundary=2 指定栈对齐方式为没有对齐
	@# -masm=intel 指定汇编代码的格式为intel格式
	gcc -m32 -S -o hello.s hello.c -fno-asynchronous-unwind-tables -fno-pic -fno-pie -mpreferred-stack-boundary=2 # -masm=intel

hello.asm:hello.c
	gcc -m32 -S -o hello.asm hello.c -fno-asynchronous-unwind-tables -fno-pic -mpreferred-stack-boundary=2 -masm=intel


PHONY:clean
clean: 
	rm -f hello.s hello.asm
