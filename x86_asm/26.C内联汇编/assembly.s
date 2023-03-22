	.file	"assembly.c"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
#APP
# 6 "assembly.c" 1
	movl $1, %eax
# 0 "" 2
# 7 "assembly.c" 1
	movl $0, %ebx
# 0 "" 2
# 8 "assembly.c" 1
	int $0x80
# 0 "" 2
#NO_APP
	movl	$0, %eax
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
