	.file	"do_while.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	$0, -4(%ebp)
	movl	$0, -8(%ebp)
.L2:
	movl	-8(%ebp), %eax
	addl	%eax, -4(%ebp)
	addl	$1, -8(%ebp)
	cmpl	$9, -8(%ebp)
	jle	.L2
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
