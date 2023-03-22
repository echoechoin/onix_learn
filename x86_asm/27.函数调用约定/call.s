	.file	"call.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 0 "/home/echo/onix/x86_asm/27.\345\207\275\346\225\260\350\260\203\347\224\250\347\272\246\345\256\232" "call.c"
	.section	.rodata
.LC0:
	.string	"%d + %d = %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.file 1 "call.c"
	.loc 1 7 1
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	.loc 1 8 9
	movl	$1, -4(%ebp)
	.loc 1 9 9
	movl	$2, -8(%ebp)
	.loc 1 10 13
	pushl	$2
	pushl	$1
	call	add
	addl	$8, %esp
	movl	%eax, -12(%ebp)
	.loc 1 11 5
	pushl	-12(%ebp)
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	.loc 1 13 14
	movl	$2, %edx
	movl	$1, %ecx
	call	add_fast
	movl	%eax, -16(%ebp)
	.loc 1 14 5
	pushl	-16(%ebp)
	pushl	-8(%ebp)
	pushl	-4(%ebp)
	pushl	$.LC0
	call	printf
	addl	$16, %esp
	.loc 1 15 12
	movl	$0, %eax
	.loc 1 16 1
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/include/stdio.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x111
	.value	0x5
	.byte	0x1
	.byte	0x4
	.long	.Ldebug_abbrev0
	.uleb128 0x4
	.long	.LASF14
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
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
	.long	.LASF12
	.byte	0x1
	.byte	0x4
	.byte	0x26
	.long	0x5c
	.long	0x93
	.uleb128 0x2
	.long	0x5c
	.uleb128 0x2
	.long	0x5c
	.byte	0
	.uleb128 0x8
	.long	.LASF13
	.byte	0x2
	.value	0x164
	.byte	0xc
	.long	0x5c
	.long	0xab
	.uleb128 0x2
	.long	0xb1
	.uleb128 0x9
	.byte	0
	.uleb128 0xa
	.byte	0x4
	.long	0x34
	.uleb128 0xb
	.long	0xab
	.uleb128 0xc
	.string	"add"
	.byte	0x1
	.byte	0x3
	.byte	0xc
	.long	0x5c
	.long	0xd1
	.uleb128 0x2
	.long	0x5c
	.uleb128 0x2
	.long	0x5c
	.byte	0
	.uleb128 0xd
	.long	.LASF15
	.byte	0x1
	.byte	0x6
	.byte	0x5
	.long	0x5c
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x3
	.string	"a"
	.byte	0x8
	.long	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x3
	.string	"b"
	.byte	0x9
	.long	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x3
	.string	"c"
	.byte	0xa
	.long	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x3
	.string	"d"
	.byte	0xd
	.long	0x5c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
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
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
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
	.uleb128 0x21
	.sleb128 9
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x5
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
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
	.uleb128 0x7
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
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
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
	.uleb128 0x9
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x37
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
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
.LASF14:
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
.LASF12:
	.string	"add_fast"
.LASF13:
	.string	"printf"
.LASF8:
	.string	"short int"
.LASF7:
	.string	"signed char"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"call.c"
.LASF1:
	.string	"/home/echo/onix/x86_asm/27.\345\207\275\346\225\260\350\260\203\347\224\250\347\272\246\345\256\232"
	.ident	"GCC: (GNU) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
