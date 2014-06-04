ENTRY (loader)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0xc0100000;

    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xc0000000)
    {
        __start_text = .;
        *(.text)
	*(.text.*)
        *(.gnu.linkonce.t*)
        __end_text = .;
    }

    .ctors ALIGN (0x1000) : AT(ADDR(.ctors) - 0xc0000000)
    {
        __start_ctors = .;
        *(SORT(.ctors))
        *(SORT(.init_array))
        __end_ctors = .;
    }

    .dtors ALIGN (0x1000) : AT(ADDR(.dtors) - 0xc0000000)
    {
        __start_dtors = .;
        *(SORT(.dtors))
	*(SORT(.fini_array))
        __end_dtors = .;
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0xc0000000)
    {
        __start_rodata = .;
        *(.rodata*)
        *(.gnu.linkonce.r*)
        __end_rodata = .;
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xc0000000)
    {
        __start_data = .;
        *(.data)
        *(.gnu.linkonce.d*)
        __end_data = .;
    }

    .bss : AT(ADDR(.bss) - 0xc0000000)
    {
        __start_bss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        __end_bss = .;
    }
    __end_kernel = .;

    /DISCARD/ :
    {
/*	*(.debug_*) */
        *(.comment)
        *(.comment.*)
        *(.eh_frame) /* C++ exceptions. */
    }
}
