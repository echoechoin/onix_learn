	.file	"global_variable.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	10
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	20
	.globl	result
	.bss
	.align 4
	.type	result, @object
	.size	result, 4
result:
	.zero	4
	.section	.rodata
.LC0:
	.string	"result = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
#APP
# 10 "global_variable.c" 1
	movl a, %eax
# 0 "" 2
# 11 "global_variable.c" 1
	movl b, %ebx
# 0 "" 2
# 12 "global_variable.c" 1
	addl %ebx, %eax
# 0 "" 2
# 13 "global_variable.c" 1
	movl %eax, result
# 0 "" 2
#NO_APP
	movl	result, %eax
	pushl	%eax
	pushl	$.LC0
	call	printf
	addl	$8, %esp
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
