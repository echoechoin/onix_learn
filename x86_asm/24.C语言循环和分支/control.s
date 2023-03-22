	.file	"control.c"
	.text
	.globl	compare
	.type	compare, @function
compare:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	$0, -4(%ebp)
	jmp	.L2
.L6:
	cmpl	$0, 8(%ebp)
	je	.L7
	nop
	addl	$1, -4(%ebp)
.L2:
	cmpl	$9, -4(%ebp)
	jle	.L6
	jmp	.L5
.L7:
	nop
.L5:
	nop
	leave
	ret
	.size	compare, .-compare
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	$3
	pushl	$2
	pushl	$1
	call	compare
	addl	$12, %esp
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
