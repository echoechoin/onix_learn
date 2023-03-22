	.file	"hello.c"
	.text
	.section	.rodata
.LC0:
	.string	"hello world!"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$.LC0
	call	puts
	addl	$4, %esp
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
