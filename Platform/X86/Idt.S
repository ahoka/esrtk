// the descriptors
.section .rodata
.align 16



idtr:
.word 0x0000000
.long idtEntries

.global idtinit
idtinit:
	movl $idtr, %eax
	lidt (%eax)
	ret
