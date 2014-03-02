#include <Assembly.hh>
#include <Parameters.hh>

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

FUNCTION(x86_idt_init)
	movl $idtPointer, %eax
	lidt (%eax)
	ret
END(x86_idt_init)
	
#include "InterruptVectors.is"

.text
.balign 16
x86_isr_vector_common:
	push_context

	incl interrupt_level

	movl $KernelDataSegment, %eax
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
	addl $4, %esp

	movl %eax, %esp

	decl interrupt_level
	
	pop_context

	// remove the error codes from the stack too
	addl $8, %esp

        sti
	iret
