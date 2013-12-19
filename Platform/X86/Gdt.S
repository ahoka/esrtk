#include <Parameters.hh>

// the descriptors
.section .rodata
.balign 16

gdt:
// null descriptor
.long 0x00000000
.long 0x00000000

// kernel code
.long 0x0000ffff
.long 0x00cf9a00

// kernel data
.long 0x0000ffff
.long 0x00cf9200

// user code
.long 0x0000ffff
.long 0x00cffa00

// user data
.long 0x0000ffff
.long 0x00cff200
egdt:

gdtr:
.word egdt - gdt - 1
.long gdt

.text
.balign 16

.global gdt_init
gdt_init:
	movl $gdtr, %eax
	lgdt (%eax)
	ljmp $KernelCodeSegment, $gdtflush
gdtflush:
	movl $KernelDataSegment, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
	movl %eax, %ss
	ret
