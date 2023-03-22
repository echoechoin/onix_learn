	.file	"compile.cc"
	.text
	.globl	function
	.type	function, @function
function:
	pushl	%ebp
	movl	%esp, %ebp
	nop
	popl	%ebp
	ret
	.size	function, .-function
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	call	function
	movl	$0, %eax
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
