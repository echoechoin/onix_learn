	.file	"local_variable.c"
	.text
	.section	.rodata
.LC0:
	.string	"result = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$12, %esp
	movl	$10, -8(%ebp)
	movl	$20, -12(%ebp)
	movl	$0, -16(%ebp)
	movl	-8(%ebp), %eax
	movl	-12(%ebp), %edx
#APP
# 12 "local_variable.c" 1
	addl %edx, %eax

# 0 "" 2
#NO_APP
	movl	%eax, -16(%ebp)
	movl	-8(%ebp), %eax
	movl	-12(%ebp), %edx
#APP
# 19 "local_variable.c" 1
	addl %eax, %edx
movl %edx, %eax

# 0 "" 2
#NO_APP
	movl	%eax, -16(%ebp)
	movl	-8(%ebp), %edx
	movl	-16(%ebp), %eax
#APP
# 28 "local_variable.c" 1
	addl %edx, %eax
movl %eax, %eax

# 0 "" 2
#NO_APP
	movl	%eax, -16(%ebp)
	movl	-8(%ebp), %edx
	movl	-16(%ebp), %eax
#APP
# 37 "local_variable.c" 1
	addl %edx, %eax
movl %eax, %eax

# 0 "" 2
#NO_APP
	movl	%eax, -16(%ebp)
	movl	-8(%ebp), %ecx
	movl	-12(%ebp), %ebx
#APP
# 46 "local_variable.c" 1
	movl %ecx, %eax
movl %ebx, %edx
addl %edx, %eax
movl %eax, %ecx

# 0 "" 2
#NO_APP
	movl	%ecx, -16(%ebp)
#APP
# 57 "local_variable.c" 1
	addl -8(%ebp), -12(%ebp)
movl -8(%ebp), -16(%ebp)

# 0 "" 2
#NO_APP
	movl	-16(%ebp), %eax
	pushl	%eax
	pushl	$.LC0
	call	printf
	addl	$8, %esp
	movl	$0, %eax
	movl	-4(%ebp), %ebx
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
