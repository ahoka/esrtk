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


// cpuid(u32 eax, u32* result)
.global cpuid
cpuid:
	pushl %ebx
	pushl %edi
	movl 12(%esp), %eax
	movl 16(%esp), %edi
	xorl %ecx, %ecx
	xorl %ebx, %ebx
	xorl %edx, %edx
	cpuid
	movl %eax, 0(%edi)
	movl %ebx, 4(%edi)
	movl %ecx, 8(%edi)
	movl %edx, 12(%edi)
	popl %edi
	popl %ebx
	ret
