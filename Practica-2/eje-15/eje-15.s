	.file	"eje-15.c"
	.text
	.globl	visitantes
	.bss
	.align 4
	.type	visitantes, @object
	.size	visitantes, 4
visitantes:
	.zero	4
	.globl	flag
	.align 32
	.type	flag, @object
	.size	flag, 4000000
flag:
	.zero	4000000
	.globl	turno
	.align 4
	.type	turno, @object
	.size	turno, 4
turno:
	.zero	4
	.text
	.globl	molinete
	.type	molinete, @function
molinete:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L2
.L5:
	movl	$1, 4+flag(%rip)
	movl	$0, turno(%rip)
#APP
# 22 "eje-15.c" 1
	mfence
# 0 "" 2
#NO_APP
	nop
.L4:
	movl	flag(%rip), %eax
	cmpl	$1, %eax
	jne	.L3
	movl	turno(%rip), %eax
	testl	%eax, %eax
	je	.L4
.L3:
	movl	visitantes(%rip), %eax
	addl	$1, %eax
	movl	%eax, visitantes(%rip)
	movl	$0, 4+flag(%rip)
	addl	$1, -4(%rbp)
.L2:
	cmpl	$999999, -4(%rbp)
	jle	.L5
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	molinete, .-molinete
	.section	.rodata
	.align 8
.LC0:
	.string	"El numero de visitantes es : %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-24(%rbp), %rax
	movl	$0, %ecx
	leaq	molinete(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	leaq	-16(%rbp), %rax
	movl	$0, %ecx
	leaq	molinete(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movq	-16(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	visitantes(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.1 20230201"
	.section	.note.GNU-stack,"",@progbits
