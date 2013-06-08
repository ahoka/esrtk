// the descriptors
.section .rodata
.align 16

gdt:
.long 0x00000000
.long 0x00000000

.long 0x0000ffff
.long 0x00cf9a00

.long 0x0000ffff
.long 0x00cf9200
egdt:

gdtr:
.word egdt - gdt - 1
.long gdt

.text
.align 16

.global initGdt
initGdt:
	movl $gdtr, %eax
	lgdt (%eax)
	ljmp $0x08, $gdtflush
gdtflush:
	movl $0x10, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
	movl %eax, %ss
	ret
