	.file	"union.c"
	.text
	.globl	udata
	.bss
	.align 4
	.type	udata, @object
	.size	udata, 8
udata:
	.zero	8
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	movb	$-86, udata
	movb	$18, udata
	movl	$305419896, udata+4
	movl	$0, %eax
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
