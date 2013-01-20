ENTRY (loader)

SECTIONS
{
    . = 0x00100000;

    .magic :
    {
	*(.magic)
    }

    .text ALIGN (0x1000) :
    {
        *(.text)
	*(.text.startup)
        *(.gnu.linkonce.t*)
    }

    .ctors ALIGN (0x1000) :
    {
        __start_ctors = .;
        *(SORT(.ctors))
        *(SORT(.init_array))
        __end_ctors = .;
    }

    .dtors ALIGN (0x1000) :
    {
        __start_dtors = .;
        *(SORT(.dtors))
	*(SORT(.fini_array))
        __end_dtors = .;
    }

    .rodata ALIGN (0x1000) :
    {
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
        *(.gnu.linkonce.d*)
    }

    .bss :
    {
        __sbss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        __ebss = .;
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.comment.*)
        *(.eh_frame) /* C++ exceptions. */
    }
}
