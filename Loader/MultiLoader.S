#include <X86/Parameters.hh>

.global loader                          # making entry point visible to linker

# setting up the Multiboot header - see GRUB docs for details
.set ALIGN,    1<<0                     # align loaded modules on page boundaries
.set MEMINFO,  1<<1                     # provide memory map
.set VBEINFO,  1<<2
.set FLAGS,    ALIGN | MEMINFO		# this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002               # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)         # checksum required

.global magic1
.global flags
#.section .magic
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

.section .data
.balign 0x1000

init_page_dir_start:
initial_page_directory:
.long 0x00000083
.rept (KernelPageNumber - 1)
.long 0x00000000
.endr
.long 0x00000083
.rept (1024 - KernelPageNumber - 1)
.long 0x00000000
.endr	
init_page_dir_end:
	
.section .text
.balign 16

.set loader, (_loader - KernelVirtualBase)
_loader:
	movl $(initial_page_directory - KernelVirtualBase), %ecx
	movl %ecx, %cr3

	# Enable 4MB pages
	movl %cr4, %ecx
	orl $0x00000010, %ecx
	movl %ecx, %cr4

	# Enable paging
	movl %cr0, %ecx
	orl $0x80000000, %ecx
	movl %ecx, %cr0

	leal (higher_half_loader), %ecx

	jmp *%ecx

higher_half_loader:
	// TODO: unmap identity mapped first page

	mov   $(stack + STACKSIZE), %esp    # set up the stack
	movl  %eax, magic                   # Multiboot magic number
	movl  %ebx, mbd                     # Multiboot data structure

	call initGdt
	call initSupervisor

	mov  $__start_ctors, %ebx             # call the constructors
	jmp  2f
1:
	call *(%ebx)
	add  $4, %ebx
2:
	cmp  $__end_ctors, %ebx
	jb   1b

	call kmain                          # call kernel proper

	mov  $__end_dtors, %ebx               # call the destructors
	jmp  4f
3:
	sub  $4, %ebx
	call *(%ebx)
4:
	cmp  $__start_dtors, %ebx
	ja   3b

	cli
hang:
	hlt                                 # halt machine should kernel return
	jmp   hang
