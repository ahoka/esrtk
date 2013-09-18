// C callable assembly functions for X86
//

.text
.balign 16

// rdmsr(in u32 address, out u32 eax, out u32 edx)
.global rdmsr
rdmsr:
        pushl %edi
	movl 8(%esp), %ecx
	rdmsr
        movl 12(%esp), %edi
        movl %eax, (%edi)
        movl 16(%esp), %edi
        movl %edx, (%edi)
        popl %edi
	ret

// wrmsr(in u32 address, in u32 eax, in u32 edx)
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

.global getCr2
getCr2:
        movl %cr2, %eax
        ret

.global setEflags
setEflags:
	movl 4(%esp), %eax
	pushl %eax
	popfl
	ret

.global setCr3
setCr3:
	movl 4(%esp), %eax
        movl %eax, %cr3
	ret

.global getCr3
getCr3:
        movl %cr3, %eax
	ret

.global getCr0
getCr0:
        movl %cr0, %eax
        ret

.global setCr0
setCr0:
        movl 4(%esp), %eax
        movl %eax, %cr0
        ret

.global getCr4
getCr4:
        movl %cr4, %eax
        ret

.global setCr4
setCr4:
        movl 4(%esp), %eax
        movl %eax, %cr4
        ret

.global invlpg
invlpg:
        movl 4(%esp), %eax
        invlpg (%eax)
        ret

.global initPaging
initPaging:
        movl %esp, %ebp
        push %edx
        movl 8(%esp), %edx

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

        pop %edx
        
        movl (%ebp), %eax
        movl %ebp, %esp

	ret

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
