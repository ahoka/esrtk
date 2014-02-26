#include <Parameters.hh>

// the descriptors
.text
.balign 16

.macro push_context
        pushl %eax
        pushl %ebx
        pushl %ecx
        pushl %edx
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
        popl %edx
        popl %ecx
        popl %ebx
        popl %eax
.endm

.global create_context
create_context:
	pushl $0x02 // eflags
	pushl $KernelCodeSegment // cs
	pushl %eax // eip
	iret

.global idtinit
idtinit:
	movl $idtPointer, %eax
	lidt (%eax)
	ret

#include "InterruptVectors.is"

call_isr:
	push_context

	incl interrupt_level

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
        
        call x86_isr_dispatcher
	
	// esp was pushed to stack for isrDispatcher,
	// so remove it
	//addl $4, %esp

	movl %eax, %esp

	decl interrupt_level
	
	pop_context

	// remove the error codes from the stack too
	addl $8, %esp

        sti
	iret
