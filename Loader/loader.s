.global loader                          # making entry point visible to linker

# setting up the Multiboot header - see GRUB docs for details
.set ALIGN,    1<<0                     # align loaded modules on page boundaries
.set MEMINFO,  1<<1                     # provide memory map
.set FLAGS,    ALIGN | MEMINFO          # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002               # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)         # checksum required

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# reserve initial kernel stack space
.set STACKSIZE, 0x4000                  # that is, 16k.

.comm stack, STACKSIZE, 32             # reserve 16k stack on a doubleword boundary
#.lcomm stack, STACKSIZE
.comm  mbd, 4                           # we will use this in kmain
.comm  magic, 4                         # we will use this in kmain

loader:
	mov   $(stack + STACKSIZE), %esp    # set up the stack
	movl  %eax, magic                   # Multiboot magic number
	movl  %ebx, mbd                     # Multiboot data structure

	mov  $start_ctors, %ebx             # call the constructors
	jmp  2f
1:
	call *(%ebx)
	add  $4, %ebx
2:
	cmp  $end_ctors, %ebx
	jb   1b

	call kmain                          # call kernel proper

	mov  $end_dtors, %ebx               # call the destructors
	jmp  4f
3:
	sub  $4, %ebx
	call *(%ebx)
4:
	cmp  $start_dtors, %ebx
	ja   3b

	cli
hang:
	hlt                                 # halt machine should kernel return
	jmp   hang
