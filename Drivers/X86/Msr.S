// C callable assembly functions for X86
//

.text
.align 16

// rdmsr(u32 address)
.global rdmsr
rdmsr:
	movl 4(%esp), %ecx
	rdmsr
	ret

// wrmsr(u32 address, u64 value)
.global wrmsr
wrmsr:	
	movl 4(%esp), %ecx
	movl 8(%esp), %eax
	movl 12(%esp), %edx
	wrmsr
	ret

