ENTRY (loader)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0xc0100000;

    .magic :
    {
	*(.magic)
    }

    .text ALIGN (0x1000) : AT(ADDR(.text) - 0xc0000000)
    {
        *(.text)
	*(.text.*)
        *(.gnu.linkonce.t*)
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
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0xc0000000)
    {
        *(.data)
        *(.gnu.linkonce.d*)
    }

    .bss : AT(ADDR(.bss) - 0xc0000000)
    {
        __sbss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        __ebss = .;
    }

    /DISCARD/ :
    {
/*	*(.debug_*) */
        *(.comment)
        *(.comment.*)
        *(.eh_frame) /* C++ exceptions. */
    }
}
