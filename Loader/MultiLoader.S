#include <X86/Parameters.hh>

# setting up the Multiboot header - see GRUB docs for details
.set ALIGN,    1<<0                     # align loaded modules on page boundaries
.set MEMINFO,  1<<1                     # provide memory map
.set VBEINFO,  1<<2
.set FLAGS,    ALIGN | MEMINFO		# this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002               # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)         # checksum required

.global magic1
.global flags
.balign 4
magic1:
.long MAGIC
flags:	
.long FLAGS
.long CHECKSUM

# reserve initial kernel stack space
.set STACKSIZE, 0x8000

.comm stack, STACKSIZE, 32             # reserve stack on a doubleword boundary
#.lcomm stack, STACKSIZE
.comm  mbd, 4                           # we will use this in kmain
.comm  magic, 4                         # we will use this in kmain

.section .rodata
.balign 0x1000

// we use a page directory with 0x100000 mappped to 0xc0100000
// and identity mapped at the same time with 4mb pages
// we will discard this page table later
//
page_directory:
.long 0x00000083
.rept (KernelPageNumber - 1)
.long 0x00000000
.endr
.long 0x00000083
.rept (1024 - KernelPageNumber - 2)
.long 0x00000000
.endr
.long ((page_directory - KernelVirtualBase) + 0x3)
	
.section .text
.balign 16

.global loader
.set loader, (_loader - KernelVirtualBase)
_loader:
	movl $(page_directory - KernelVirtualBase), %ecx
	movl %ecx, %cr3

	# Enable 4MB pages
	movl %cr4, %ecx
	orl $0x00000010, %ecx
	movl %ecx, %cr4

	# Enable paging
	movl %cr0, %ecx
	orl $0x80000000, %ecx
	movl %ecx, %cr0

	leal (hiloader), %ecx
	jmp *%ecx

hiloader:
	mov   $(stack + STACKSIZE), %esp    # set up the stack
	movl  %eax, magic                   # Multiboot magic number
	movl  %ebx, mbd                     # Multiboot data structure

        // reset eflags
        pushl %eax
        movl $0x2, %eax
        pushl %eax
        popfl
     
	call kmain

	cli
hang:
	hlt
	jmp   hang
