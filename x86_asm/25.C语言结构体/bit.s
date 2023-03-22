	.file	"bit.c"
	.text
	.globl	data
	.bss
	.type	data, @object
	.size	data, 1
data:
	.zero	1
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	movzbl	data, %eax
	andl	$-16, %eax
	orl	$10, %eax
	movb	%al, data
	movzbl	data, %eax
	andl	$-49, %eax
	orl	$32, %eax
	movb	%al, data
	movzbl	data, %eax
	orl	$64, %eax
	movb	%al, data
	movzbl	data, %eax
	andl	$127, %eax
	movb	%al, data
	movl	$0, %eax
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
