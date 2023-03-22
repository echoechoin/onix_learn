	.file	"cstring.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/echo/onix/x86_asm/29.\345\255\227\347\254\246\344\270\262\345\244\204\347\220\206" "cstring.c"
	.globl	memcpy
	.type	memcpy, @function
memcpy:
.LFB0:
	.file 1 "cstring.c"
	.loc 1 12 1
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$12, %esp
	.loc 1 13 11
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	.loc 1 14 17
	movl	12(%ebp), %eax
	movl	%eax, -12(%ebp)
.LBB2:
	.loc 1 15 17
	movl	$0, -4(%ebp)
	.loc 1 15 5
	jmp	.L2
.L3:
	.loc 1 17 23 discriminator 3
	movl	-12(%ebp), %edx
	movl	-4(%ebp), %eax
	addl	%edx, %eax
	.loc 1 17 13 discriminator 3
	movl	-8(%ebp), %ecx
	movl	-4(%ebp), %edx
	addl	%ecx, %edx
	.loc 1 17 23 discriminator 3
	movzbl	(%eax), %eax
	.loc 1 17 17 discriminator 3
	movb	%al, (%edx)
	.loc 1 15 36 discriminator 3
	addl	$1, -4(%ebp)
.L2:
	.loc 1 15 26 discriminator 1
	movl	-4(%ebp), %eax
	cmpl	16(%ebp), %eax
	jb	.L3
.LBE2:
	.loc 1 19 12
	movl	8(%ebp), %eax
	.loc 1 20 1
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	memcpy, .-memcpy
	.globl	message
	.data
	.align 4
	.type	message, @object
	.size	message, 12
message:
	.string	"hello world"
	.globl	buffer1
	.align 4
	.type	buffer1, @object
	.size	buffer1, 12
buffer1:
	.string	"\001"
	.zero	10
	.globl	buffer2
	.bss
	.align 4
	.type	buffer2, @object
	.size	buffer2, 12
buffer2:
	.zero	12
	.section	.rodata
.LC0:
	.string	"buffer1: %d\n"
.LC1:
	.string	"buffer1: %s\n"
.LC2:
	.string	"buffer2: %s\n"
.LC3:
	.string	"ret: %d\n"
.LC4:
	.string	"p: %p, c: %c\n"
.LC5:
	.string	"not found\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.loc 1 27 1
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	.loc 1 28 5
	pushl	$12
	pushl	$0
	pushl	$buffer1
	call	memset_s
	addl	$12, %esp
	.loc 1 29 36
	movzbl	buffer1, %eax
	.loc 1 29 5
	movsbl	%al, %eax
	pushl	%eax
	pushl	$.LC0
	call	printf
	addl	$8, %esp
	.loc 1 31 5
	pushl	$12
	pushl	$message
	pushl	$buffer1
	call	memcpy
	addl	$12, %esp
	.loc 1 32 5
	pushl	$12
	pushl	$message
	pushl	$buffer2
	call	memcpy_s
	addl	$12, %esp
	.loc 1 33 5
	pushl	$buffer1
	pushl	$.LC1
	call	printf
	addl	$8, %esp
	.loc 1 34 5
	pushl	$buffer2
	pushl	$.LC2
	call	printf
	addl	$8, %esp
	.loc 1 36 16
	movb	$47, buffer2+5
	.loc 1 37 15
	pushl	$12
	pushl	$buffer2
	pushl	$buffer1
	call	memcmp_s
	addl	$12, %esp
	movl	%eax, -4(%ebp)
	.loc 1 38 5
	pushl	-4(%ebp)
	pushl	$.LC3
	call	printf
	addl	$8, %esp
	.loc 1 40 15
	pushl	$12
	pushl	$119
	pushl	$buffer1
	call	memchr_s
	addl	$12, %esp
	movl	%eax, -8(%ebp)
	.loc 1 41 8
	cmpl	$0, -8(%ebp)
	je	.L6
	.loc 1 43 37
	movl	-8(%ebp), %eax
	movzbl	(%eax), %eax
	.loc 1 43 9
	movsbl	%al, %eax
	pushl	%eax
	pushl	-8(%ebp)
	pushl	$.LC4
	call	printf
	addl	$12, %esp
	jmp	.L7
.L6:
	.loc 1 47 9
	pushl	$.LC5
	call	printf
	addl	$4, %esp
.L7:
	.loc 1 50 12
	movl	$0, %eax
	.loc 1 51 1
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/include/stdio.h"
	.file 3 "/usr/lib/gcc/x86_64-pc-linux-gnu/12.2.0/include/stddef.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x213
	.value	0x5
	.byte	0x1
	.byte	0x4
	.long	.Ldebug_abbrev0
	.uleb128 0x9
	.long	.LASF26
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0xa
	.long	.LASF27
	.byte	0x3
	.byte	0xd6
	.byte	0x17
	.long	0x32
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF3
	.uleb128 0xb
	.long	0x39
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF5
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF6
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF8
	.uleb128 0xc
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF10
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF11
	.uleb128 0xd
	.byte	0x4
	.uleb128 0x4
	.long	0x39
	.uleb128 0xe
	.long	0x39
	.long	0x9b
	.uleb128 0xf
	.long	0x32
	.byte	0xb
	.byte	0
	.uleb128 0x5
	.long	.LASF12
	.byte	0x16
	.long	0x8b
	.uleb128 0x5
	.byte	0x3
	.long	message
	.uleb128 0x5
	.long	.LASF13
	.byte	0x17
	.long	0x8b
	.uleb128 0x5
	.byte	0x3
	.long	buffer1
	.uleb128 0x5
	.long	.LASF14
	.byte	0x18
	.long	0x8b
	.uleb128 0x5
	.byte	0x3
	.long	buffer2
	.uleb128 0x3
	.long	.LASF15
	.byte	0x9
	.byte	0x7
	.long	0x84
	.long	0xea
	.uleb128 0x1
	.long	0xea
	.uleb128 0x1
	.long	0x68
	.uleb128 0x1
	.long	0x26
	.byte	0
	.uleb128 0x4
	.long	0xef
	.uleb128 0x10
	.uleb128 0x3
	.long	.LASF16
	.byte	0x8
	.byte	0x5
	.long	0x68
	.long	0x10f
	.uleb128 0x1
	.long	0xea
	.uleb128 0x1
	.long	0xea
	.uleb128 0x1
	.long	0x26
	.byte	0
	.uleb128 0x3
	.long	.LASF17
	.byte	0x6
	.byte	0x7
	.long	0x84
	.long	0x12e
	.uleb128 0x1
	.long	0x84
	.uleb128 0x1
	.long	0xea
	.uleb128 0x1
	.long	0x26
	.byte	0
	.uleb128 0x11
	.long	.LASF18
	.byte	0x2
	.value	0x164
	.byte	0xc
	.long	0x68
	.long	0x146
	.uleb128 0x1
	.long	0x14b
	.uleb128 0x12
	.byte	0
	.uleb128 0x4
	.long	0x40
	.uleb128 0x13
	.long	0x146
	.uleb128 0x3
	.long	.LASF19
	.byte	0x7
	.byte	0x7
	.long	0x84
	.long	0x16f
	.uleb128 0x1
	.long	0x84
	.uleb128 0x1
	.long	0x68
	.uleb128 0x1
	.long	0x26
	.byte	0
	.uleb128 0x14
	.long	.LASF20
	.byte	0x1
	.byte	0x1a
	.byte	0x5
	.long	0x68
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x1a4
	.uleb128 0x6
	.string	"ret"
	.byte	0x25
	.byte	0x9
	.long	0x68
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x6
	.string	"p"
	.byte	0x28
	.byte	0xb
	.long	0x86
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x15
	.long	.LASF21
	.byte	0x1
	.byte	0xb
	.byte	0x7
	.long	0x84
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x7
	.long	.LASF22
	.byte	0x14
	.long	0x84
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x16
	.string	"src"
	.byte	0x1
	.byte	0xb
	.byte	0x26
	.long	0xea
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x7
	.long	.LASF23
	.byte	0x32
	.long	0x26
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x8
	.long	.LASF24
	.byte	0xd
	.byte	0xb
	.long	0x86
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x8
	.long	.LASF25
	.byte	0xe
	.byte	0x11
	.long	0x146
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x17
	.long	.LBB2
	.long	.LBE2-.LBB2
	.uleb128 0x6
	.string	"i"
	.byte	0xf
	.byte	0x11
	.long	0x26
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2
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
	.uleb128 0x3
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
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
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 4
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
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
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0x21
	.sleb128 11
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x9
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
	.uleb128 0xa
	.uleb128 0x16
	.byte	0
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
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x11
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
	.uleb128 0x12
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x37
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7a
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
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
.LASF12:
	.string	"message"
.LASF19:
	.string	"memset_s"
.LASF27:
	.string	"size_t"
.LASF15:
	.string	"memchr_s"
.LASF23:
	.string	"count"
.LASF17:
	.string	"memcpy_s"
.LASF22:
	.string	"dest"
.LASF4:
	.string	"unsigned char"
.LASF6:
	.string	"long unsigned int"
.LASF5:
	.string	"short unsigned int"
.LASF16:
	.string	"memcmp_s"
.LASF20:
	.string	"main"
.LASF2:
	.string	"unsigned int"
.LASF10:
	.string	"long long unsigned int"
.LASF24:
	.string	"dptr"
.LASF25:
	.string	"sptr"
.LASF9:
	.string	"long long int"
.LASF21:
	.string	"memcpy"
.LASF18:
	.string	"printf"
.LASF26:
	.string	"GNU C17 12.2.0 -m32 -mpreferred-stack-boundary=2 -mtune=generic -march=x86-64 -g -O0 -fno-asynchronous-unwind-tables -fno-pic -fno-builtin -fno-stack-protector"
.LASF8:
	.string	"short int"
.LASF13:
	.string	"buffer1"
.LASF14:
	.string	"buffer2"
.LASF11:
	.string	"long int"
.LASF3:
	.string	"char"
.LASF7:
	.string	"signed char"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"cstring.c"
.LASF1:
	.string	"/home/echo/onix/x86_asm/29.\345\255\227\347\254\246\344\270\262\345\244\204\347\220\206"
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
