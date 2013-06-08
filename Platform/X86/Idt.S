// the descriptors
.section .rodata
.align 16

.macro push_context
	pusha
	push %ds
	push %es
	push %fs
	push %gs
.endm

.macro pop_context
	pop %ds
	pop %es
	pop %fs
	pop %gs
	popa
.endm

.global idtinit
idtinit:
	movl $idtPointer, %eax
	lidt (%eax)
	ret

.global isr0
isr0:
	cli
	pushw $0
	pushw $0

	jmp call_isr

call_isr:
	push_context
	
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs

	push %eax
	call isrDispatcher
	pop %eax

	pop_context

	// this is wrong!
	add %esp, 8	
	iret
