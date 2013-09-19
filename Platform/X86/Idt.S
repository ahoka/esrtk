// the descriptors
.text
.balign 16

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

#include "InterruptVectors.is"

call_isr:
	push_context

	movl $0x10, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs

        movl %esp, %eax
        pushl %eax

        // clear ebp to make call chains sane in
        // interrupt handlers
        xorl %ebp, %ebp
        
        call isrDispatcher
	popl %esp

	pop_context

	addl $8, %esp

        sti
	iret
