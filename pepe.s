	.file	"pepe.c"
	.text
	.globl	count
	.data
	.align 4
	.type	count, @object
	.size	count, 4
count:
	.long	1
	.globl	terminated
	.align 4
	.type	terminated, @object
	.size	terminated, 4
terminated:
	.long	1
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	jmp	.L2
.L4:
	movl	count(%rip), %eax
	movl	%eax, %edi
	call	printint@PLT
	movl	count(%rip), %eax
	addl	$1, %eax
	movl	%eax, count(%rip)
.L2:
	movl	count(%rip), %eax
	cmpl	$9, %eax
	jg	.L3
	movl	terminated(%rip), %eax
	testl	%eax, %eax
	je	.L4
.L3:
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
