OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

MEMORY
{
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM(xrw)  : ORIGIN = 0x20000000, LENGTH =  128K
    CCM(rwx)  : ORIGIN = 0x10000000, LENGTH =   64K   /* core coupled memory   */
    NVRAM(wr) : ORIGIN = 0x40024000, LENGTH =    4k   /* battery backed memory (BKPSRAM) */
}

PROVIDE(_sram = ORIGIN(RAM));
PROVIDE(_eram = ORIGIN(RAM) + LENGTH(RAM));

PROVIDE(_sccm = ORIGIN(CCM));
PROVIDE(_eccm = ORIGIN(CCM) + LENGTH(CCM));

PROVIDE(_snvram = ORIGIN(NVRAM));
PROVIDE(_envram = ORIGIN(NVRAM) + LENGTH(NVRAM));

/* PROVIDE(_stack_top = _eram); */
PROVIDE(_stack_top = _eccm);

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
    } >FLASH

    .text : {
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;

        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;

        __fini_array_start = .;
        KEEP(*(.fini_array))
        __fini_array_end = .;

        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata*)

        *(.eh_frame_hdr) /* call frame information (exception handling) */
        *(.eh_frame)     /* call frame information (exception handling) */
        *(.gcc_except_table)

        *(.glue_7)  /* ARM code calling Thumb code */
        *(.glue_7t) /* Thumb code calling ARM code */
        . = ALIGN(4);
    } >FLASH


    /* exception handling/unwind - some Newlib functions (in common with C++ and STDC++) use this. */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH
    __exidx_end = .;

    _etext = .;

    .data : AT(_etext)
    {
        _data_lma = LOADADDR(.data);  /* load memory address */
        . = ALIGN(4);
        _sdata = .;
/*        KEEP( *(.data) )
        KEEP( *(.data.*) ) */
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
    } >RAM

    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
         *(.bss)
         *(.bss.*)
         *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        _end = .;
        PROVIDE(end = .);
    } >RAM


/* TODO: play around with CCM ram
    .ccm (NOLOAD) :
    {
        .= ALIGN(4);
        *(.ccm)
        .= ALIGN(4);
    } >CCM
*/

/* TODO: play around with non-volatile ram

    .nvram (NOLOAD) : ALIGN(4)
    {
        _snvram = .;
        *(.nvram)
        *(.nvram.*)
        _esvram = .;
    } >NVRAM

*/

/*
    PROVIDE(_sheap = _ebss);
    PROVIDE(_eheap = ALIGN(ORIGIN(RAM) + LENGTH(RAM) - 8 ,8) );
*/

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    /* Stabs debugging sections.  */
    .stab           0 : { *(.stab) }
    .stabstr        0 : { *(.stabstr) }
    .stab.excl      0 : { *(.stab.excl) }
    .stab.exclstr   0 : { *(.stab.exclstr) }
    .stab.index     0 : { *(.stab.index) }
    .stab.indexstr  0 : { *(.stab.indexstr) }
    .comment        0 : { *(.comment) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    .debug_ranges   0 : { *(.debug_ranges) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
}
