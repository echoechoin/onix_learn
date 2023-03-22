	.file	"float_c.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/echo/onix/x86_asm/32.\346\265\256\347\202\271\346\225\260\347\232\204\350\277\220\347\256\227" "float_c.c"
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	1110350234
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	22
	.globl	c
	.bss
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.zero	4
	.section	.rodata
.LC0:
	.string	"c = %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.file 1 "float_c.c"
	.loc 1 8 1
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-8, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$12, %esp
	.loc 1 9 15
	flds	a
	movl	b, %eax
	movl	%eax, -12(%ebp)
	fildl	-12(%ebp)
	fdivrp	%st, %st(1)
	.loc 1 9 11
	fstps	c
	.loc 1 10 9
	flds	c
	leal	-8(%esp), %esp
	fstpl	(%esp)
	pushl	$.LC0
	call	printf
	addl	$12, %esp
	.loc 1 11 16
	movl	$0, %eax
	.loc 1 12 1
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/include/stdio.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xe2
	.value	0x5
	.byte	0x1
	.byte	0x4
	.long	.Ldebug_abbrev0
	.uleb128 0x3
	.long	.LASF13
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.long	.LASF3
	.uleb128 0x4
	.long	0x2d
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.long	.LASF4
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.long	.LASF5
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.long	.LASF6
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.long	.LASF8
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x1
	.byte	0x4
	.byte	0x5
	.long	.LASF11
	.uleb128 0x2
	.string	"a"
	.byte	0x3
	.byte	0x7
	.long	0x87
	.uleb128 0x5
	.byte	0x3
	.long	a
	.uleb128 0x1
	.byte	0x4
	.byte	0x4
	.long	.LASF12
	.uleb128 0x2
	.string	"b"
	.byte	0x4
	.byte	0x5
	.long	0x5c
	.uleb128 0x5
	.byte	0x3
	.long	b
	.uleb128 0x2
	.string	"c"
	.byte	0x5
	.byte	0x7
	.long	0x87
	.uleb128 0x5
	.byte	0x3
	.long	c
	.uleb128 0x6
	.long	.LASF14
	.byte	0x2
	.value	0x164
	.byte	0xc
	.long	0x5c
	.long	0xc4
	.uleb128 0x7
	.long	0xca
	.uleb128 0x8
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.long	0x34
	.uleb128 0xa
	.long	0xc4
	.uleb128 0xb
	.long	.LASF15
	.byte	0x1
	.byte	0x7
	.byte	0x5
	.long	0x5c
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x37
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.value	0
	.value	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF9:
	.string	"long long int"
.LASF5:
	.string	"short unsigned int"
.LASF2:
	.string	"unsigned int"
.LASF13:
	.string	"GNU C17 12.2.0 -m32 -mpreferred-stack-boundary=2 -mtune=generic -march=x86-64 -g -O0 -fno-asynchronous-unwind-tables -fno-pic -fno-builtin -fno-stack-protector"
.LASF6:
	.string	"long unsigned int"
.LASF10:
	.string	"long long unsigned int"
.LASF3:
	.string	"char"
.LASF4:
	.string	"unsigned char"
.LASF15:
	.string	"main"
.LASF11:
	.string	"long int"
.LASF14:
	.string	"printf"
.LASF12:
	.string	"float"
.LASF8:
	.string	"short int"
.LASF7:
	.string	"signed char"
	.section	.debug_line_str,"MS",@progbits,1
.LASF1:
	.string	"/home/echo/onix/x86_asm/32.\346\265\256\347\202\271\346\225\260\347\232\204\350\277\220\347\256\227"
.LASF0:
	.string	"float_c.c"
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
