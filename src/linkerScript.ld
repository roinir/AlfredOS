ENTRY(_start)

SECTIONS
{
    /* Load address of the kernel */
    . = 0x0100000;

    /* Code section */
    .text ALIGN(4096) :
    {
        *(.text)
    }

    /* Read-only data */
    .rodata ALIGN(4096) :
    {
        *(.rodata)
    }

    /* Data section */
    .data ALIGN(4096) :
    {
        *(.data)
    }

    /* BSS section */
    .bss ALIGN(4096) :
    {
        *(COMMON)
        *(.bss)
    }
}

