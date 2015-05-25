ENTRY (start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0x10000000;

    .text ALIGN (0x1000) : AT(ADDR(.text) - 0x10000000)
    {
        __rt_start_text = .;
        *(.text)
	*(.text.*)
        *(.gnu.linkonce.t*)
        __rt_end_text = .;
    }

    .ctors ALIGN (0x1000) : AT(ADDR(.ctors) - 0x10000000)
    {
        __rt_start_ctors = .;
        *(SORT(.ctors))
        *(SORT(.init_array))
        __rt_end_ctors = .;
    }

    .dtors ALIGN (0x1000) : AT(ADDR(.dtors) - 0x10000000)
    {
        __rt_start_dtors = .;
        *(SORT(.dtors))
	*(SORT(.fini_array))
        __rt_end_dtors = .;
    }

    .rodata ALIGN (0x1000) : AT(ADDR(.rodata) - 0x10000000)
    {
        __rt_start_rodata = .;
        *(.rodata*)
        *(.gnu.linkonce.r*)
        __rt_end_rodata = .;
    }

    .data ALIGN (0x1000) : AT(ADDR(.data) - 0x10000000)
    {
        __rt_start_data = .;
        *(.data)
        *(.gnu.linkonce.d*)
        __rt_end_data = .;
    }

    .bss : AT(ADDR(.bss) - 0x10000000)
    {
        __rt_start_bss = .;
        *(COMMON)
        *(.bss)
        *(.gnu.linkonce.b*)
        __rt_end_bss = .;
    }
    __end_runtime = .;

    /DISCARD/ :
    {
/*	*(.debug_*) */
        *(.comment)
        *(.comment.*)
        *(.eh_frame) /* C++ exceptions. */
    }
}
