	.file	"stack_protect.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	leal	-5(%ebp), %eax
	movl	$1819043144, (%eax)
	movl	$1998597231, 4(%eax)
	movl	$1919907616, 6(%eax)
	movl	$2188396, 10(%eax)
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
