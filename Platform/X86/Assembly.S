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

// u32 cpuid0(char* vendorid)
.global cpuid0
cpuid0:
	pushl %ebx
	pushl %edi
	movl 12(%esp), %edi
	xorl %eax, %eax
	xorl %ecx, %ecx
	xorl %ebx, %ebx
	xorl %edx, %edx
	cpuid
	movl %ebx, 0(%edi)
	movl %edx, 4(%edi)
	movl %ecx, 8(%edi)
	movb $0, 12(%edi)
	popl %edi
	popl %ebx
	ret

.global getEflags
getEflags:
	pushfl
	popl %eax
	ret


.global setEflags
setEflags:
	movl 4(%esp), %eax
	pushl %eax
	popfl
	ret

