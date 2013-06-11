// the descriptors
.section .rodata
.align 16

.macro push_context
        pushl %eax
        pushl %ecx
        pushl %edx
        pushl %ebx
        pushl %esp
        pushl %ebp
        pushl %esi
        pushl %edi
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
.endm

.macro pop_context
	popl %gs
	popl %fs
	popl %es
	popl %ds
        popl %edi
        popl %esi
        popl %ebp
        popl %esp
        popl %ebx
        popl %edx
        popl %ecx
        popl %eax
.endm

.global idtinit
idtinit:
	movl $idtPointer, %eax
	lidt (%eax)
	ret

.global isr0
isr0:
	cli
	pushl $256
	pushl $99

	jmp call_isr

.global isr1
isr1:
	cli
	pushl $0
	pushl $1

	jmp call_isr


.global isr2
isr2:
	cli
	pushl $0
	pushl $2

	jmp call_isr

.global isr3
isr3:
	cli
	pushl $0
	pushl $3

	jmp call_isr

.global isr4
isr4:
	cli
	pushl $0
	pushl $4

	jmp call_isr

.global isr5
isr5:
	cli
	pushl $0
	pushl $5

	jmp call_isr

.global isr6
isr6:
	cli
	pushl $0
	pushl $6

	jmp call_isr

.global isr7
isr7:
	cli
	pushl $0
	pushl $7

	jmp call_isr

.global isr8
isr8:
	cli
	pushl $8

	jmp call_isr

.global isr9
isr9:
	cli
	pushl $0
	pushl $9

	jmp call_isr

.global isr10
isr10:
	cli
	pushl $10

	jmp call_isr

.global isr11
isr11:
	cli
	pushl $11

	jmp call_isr

.global isr12
isr12:
	cli
	pushl $12

	jmp call_isr

.global isr13
isr13:
	cli
	pushl $13

	jmp call_isr

.global isr14
isr14:
	cli
	pushl $14

	jmp call_isr

.global isr16
isr16:
	cli
        pushl $0
	pushl $16

	jmp call_isr

        
call_isr:
	push_context
	
	movl $0x10, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs

        movl %esp, %eax
        pushl %eax
        call isrDispatcher

	pop_context

	add %esp, 8
	iret
