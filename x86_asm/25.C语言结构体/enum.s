	.file	"enum.c"
	.text
	.globl	data1
	.data
	.align 4
	.type	data1, @object
	.size	data1, 4
data1:
	.long	1
	.globl	data2
	.align 4
	.type	data2, @object
	.size	data2, 4
data2:
	.long	345
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
