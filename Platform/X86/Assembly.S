#include <Assembly.hh>

// C callable assembly functions for X86
//
FUNCTION(cli)
        cli
        ret
END(cli)

FUNCTION(sti)
        sti
        ret
END(sti)

// rdmsr(in u32 address, out u32 eax, out u32 edx)
FUNCTION(rdmsr)
        pushl %edi
	movl 8(%esp), %ecx
	rdmsr
        movl 12(%esp), %edi
        movl %eax, (%edi)
        movl 16(%esp), %edi
        movl %edx, (%edi)
        popl %edi
	ret
END(rdmsr)
	
// wrmsr(in u32 address, in u32 eax, in u32 edx)
FUNCTION(wrmsr)
	movl 4(%esp), %ecx
	movl 8(%esp), %eax
	movl 12(%esp), %edx
	wrmsr
	ret
END(wrmsr)

// rdtsc, serialized with cpuid instruction
FUNCTION(rdtsc)
	pushl %ebx
	pushl %edi
        xorl %eax, %eax
        cpuid
	rdtsc
        popl %edi
	popl %ebx
	ret
END(rdtsc)

// cpuid(u32 eax, u32* result)
FUNCTION(cpuid)
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
END(cpuid)

// u32 cpuid0(char* vendorid)
FUNCTION(cpuid0)
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
END(cpuid0)

FUNCTION(get_eflags)
	pushfl
	popl %eax
	ret
END(get_eflags)

FUNCTION(get_cr2)
        movl %cr2, %eax
        ret
END(get_cr2)

FUNCTION(set_eflags)
	movl 4(%esp), %eax
	pushl %eax
	popfl
	ret
END(set_eflags)

FUNCTION(set_cr3)
	movl 4(%esp), %eax
        movl %eax, %cr3
	ret
END(set_cr3)

FUNCTION(get_cr3)
        movl %cr3, %eax
	ret
END(get_cr3)

FUNCTION(get_cr0)
        movl %cr0, %eax
        ret
END(get_cr0)

FUNCTION(set_cr0)
        movl 4(%esp), %eax
        movl %eax, %cr0
        ret
END(set_cr0)

FUNCTION(get_cr4)
        movl %cr4, %eax
        ret
END(get_cr4)

FUNCTION(set_cr4)
        movl 4(%esp), %eax
        movl %eax, %cr4
        ret
END(set_cr4)

FUNCTION(invlpg)
        movl 4(%esp), %eax
        invlpg (%eax)
        ret
END(invlpg)

FUNCTION(get_eip)
	popl %eax
	jmpl *%eax
END(get_eip)

FUNCTION(x86_init_paging)
        movl 4(%esp), %edx

        movl %cr0, %eax
        andl $(~(1 << 31)), %eax
        movl %eax, %cr0

        movl %cr4, %eax
        andl $(~(1 << 4)), %eax
        movl %eax, %cr4

        movl %edx, %cr3
        
        movl %cr0, %eax
        orl $(1 << 31), %eax
        movl %eax, %cr0

	ret
END(x86_init_paging)

// not yet finised
#if 0
	
rgdt:
.long 0x00000000
.long 0x00000000

.long 0x0000ffff
.long 0x008f9a00

.long 0x0000ffff
.long 0x008f9200
ergdt:

rgdtr:
.word ergdt - rgdt - 1
.long rgdt

biosidt:
.word 0x3ff
.long 0x0000000

.global bios16
bios16:
	cli
	
        movl %cr0, %eax
        andl $(~(1 << 31)), %eax
        movl %eax, %cr0

	movl $rgdtr, %eax
	lgdt (%eax)

	movl $0x10, %eax
	movl %eax, %ds
	movl %eax, %ss
	movl %eax, %es

	ljmp $0x08, $p16
.code16
p16:
	// we are in 16 bit pmode
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0

	// and in real mode now
	xorl %eax, %eax
	movl %eax, %ds
	movl %eax, %ss
	movl %eax, %es

	movl $biosidt, %eax
	lidt (%eax)
	
        movl %cr0, %eax
        orl $(1 << 31), %eax
        movl %eax, %cr0

	sti
	ret
#endif
