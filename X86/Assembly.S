#include <Assembly.hh>

// C callable assembly functions for X86
//
FUNCTION(x86_cli)
        cli
        ret
END(x86_cli)

FUNCTION(x86_sti)
        sti
        ret
END(x86_sti)

// rdmsr(in u32 address, out u32 eax, out u32 edx)
//FUNCTION(x86_rdmsr)
//        pushl %edi
//	movl 8(%esp), %ecx
//	rdmsr
//        movl 12(%esp), %edi
//        movl %eax, (%edi)
//        movl 16(%esp), %edi
//        movl %edx, (%edi)
//        popl %edi
//	ret
//END(x86_rdmsr)
FUNCTION(x86_rdmsr)
	movl 4(%esp), %ecx
	rdmsr
	ret
END(x86_rdmsr)

FUNCTION(x86_rdpmc)
	movl 4(%esp), %ecx
	rdpmc
	ret
END(x86_rdpmc)

// wrmsr(in u32 address, in u32 eax, in u32 edx)
//FUNCTION(x86_wrmsr)
//	movl 4(%esp), %ecx
//	movl 8(%esp), %eax
//	movl 12(%esp), %edx
//	wrmsr
//	ret
//END(x86_wrmsr)
FUNCTION(x86_wrmsr)
	movl 4(%esp), %ecx
	movl 8(%esp), %eax
	movl 12(%esp), %edx
	wrmsr
	ret
END(x86_wrmsr)

// rdtsc, serialized with cpuid instruction
FUNCTION(x86_rdtsc)
	pushl %ebx
	pushl %edi
        xorl %eax, %eax
        cpuid
	rdtsc
        popl %edi
	popl %ebx
	ret
END(x86_rdtsc)

// cpuid(u32 eax, u32* result)
FUNCTION(x86_cpuid)
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
END(x86_cpuid)

// u32 cpuid0(char* vendorid)
FUNCTION(x86_cpuid0)
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
END(x86_cpuid0)

FUNCTION(x86_get_eflags)
	pushfl
	popl %eax
	ret
END(x86_get_eflags)

FUNCTION(x86_get_cr2)
        movl %cr2, %eax
        ret
END(x86_get_cr2)

FUNCTION(x86_set_eflags)
	movl 4(%esp), %eax
	pushl %eax
	popfl
	ret
END(x86_set_eflags)

FUNCTION(x86_set_cr3)
	movl 4(%esp), %eax
        movl %eax, %cr3
	ret
END(x86_set_cr3)

FUNCTION(x86_get_cr3)
        movl %cr3, %eax
	ret
END(x86_get_cr3)

FUNCTION(x86_get_cr0)
        movl %cr0, %eax
        ret
END(x86_get_cr0)

FUNCTION(x86_set_cr0)
        movl 4(%esp), %eax
        movl %eax, %cr0
        ret
END(x86_set_cr0)

FUNCTION(x86_get_cr4)
        movl %cr4, %eax
        ret
END(x86_get_cr4)

FUNCTION(x86_set_cr4)
        movl 4(%esp), %eax
        movl %eax, %cr4
        ret
END(x86_set_cr4)

FUNCTION(x86_invlpg)
        movl 4(%esp), %eax
        invlpg (%eax)
        ret
END(x86_invlpg)

FUNCTION(x86_get_eip)
	popl %eax
	jmpl *%eax
END(x86_get_eip)

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
