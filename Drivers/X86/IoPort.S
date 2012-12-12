// IO Port handing routines in assembly
//

.text
.align 16

.global inb
inb:
	movl 4(%esp), %edx
	xorl %eax, %eax
	inb %dx, %al
	ret

.global inw
inw:
	movl 4(%esp), %edx
	xorl %eax, %eax
	inw %dx, %ax
	ret

.global inl
inl:
	movl 4(%esp), %edx
	inl %dx, %eax
	ret

.global outb
outb:
	movl 4(%esp), %edx
	movl 8(%esp), %eax
	outb %al, %dx
	ret

.global outw
outw:
	movl 4(%esp), %edx
	movl 8(%esp), %eax
	outw %ax, %dx
	ret

.global outl
outl:
	movl 4(%esp), %edx
	movl 8(%esp), %eax
	outl %eax, %dx
	ret
