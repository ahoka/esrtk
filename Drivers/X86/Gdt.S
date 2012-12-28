#if 0
// the descriptors
.section rodata
.align 16

.global gdtEntries
gdtEntries:
.word 0x0 # segment limit
.word 0x0 # base low
.byte 0x0 # base middle
.byte 0x0 # attributes 1
.byte 0x0 # attributes 2
.byte 0x0 # base high

.word 0xffff # segment limit
.word 0x0 # base low
.byte 0x0 # base middle
.byte 0x0 # attributes 1
.byte 0x0 # attributes 2
.byte 0x0 # base high

.word 0xffff # segment limit
.word 0x0 # base low
.byte 0x0 # base middle
.byte 0x0 # attributes 1
.byte 0x0 # attributes 2
.byte 0x0 # base high
#endif

.text
.align 16

.global setupGdt
setupGdt:
	nop
