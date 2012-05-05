/* Linker script for Cortex-M3 MB9BF618T
 * Based on STM32F107VCT6's LinkerScript
 * 2010202, Nemui Trinomius,  BSD-LISENCE
 */
OUTPUT_FORMAT ("elf32-littlearm")

/* Memory Spaces Definitions */
MEMORY
{
	FLASH	(rx) : ORIGIN = 0x00000000, LENGTH = 1024K	/* Main Embedded FlashROM				*/
	RAM0    (xrw): ORIGIN = 0x1FFF0000, LENGTH = 64k	/* Main Embedded SRAM (I/D Code Bus) 	*/
	RAM1    (xrw): ORIGIN = 0x20000000, LENGTH = 64k	/* Main Embedded SRAM (SystemBus)		*/
}

/* higher address of the user mode stack */
_estack = ORIGIN(RAM0)+LENGTH(RAM0);


/* Sections Definitions */

SECTIONS
{
    /* for Cortex devices, the beginning of the startup code is stored in the 
	                               .isr_vector section, which goes to Flash*/
    /* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
		KEEP(*(.isr_vector))		/* Interrupt Vectors */
		KEEP(*(.boot))	  		 	/* Bootup Sequence */
        *(.text .text.*)			/* remaining code */
        *(.gnu.linkonce.t.*)
        *(.glue_7)
        *(.glue_7t)
		*(.vfp11_veneer)
		*(.janus_2cc_veneer)
		*(.v4_bx)
        *(.gcc_except_table)
        *(.rodata)					/* read-only data (constants) */
        *(.rodata*)
		KEEP(*(.ioview .ioview.*))	/* I/O View */
        *(.gnu.linkonce.r.*)
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	    . = ALIGN(4);
        
    } >FLASH

	/* .ctors .dtors are used for c++ constructors/destructors */
	/* added by Martin Thomas 4/2005 based on Anglia Design example */
	.ctors :
	{
		PROVIDE(__ctors_start__ = .);
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))
		. = ALIGN(4);
		PROVIDE(__ctors_end__ = .);
		. = ALIGN(4);
	} >FLASH
	
	.dtors :
	{
		PROVIDE(__dtors_start__ = .); 
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))
		. = ALIGN(4);
		PROVIDE(__dtors_end__ = .);
		. = ALIGN(4);
	} >FLASH
    
    . = ALIGN(4);
	/* mthomas - end */
 

	.eh_frame_hdr : ALIGN (4)
	{
		KEEP (*(.eh_frame_hdr))
	} >FLASH
	.eh_frame : ALIGN (4)
	{
	KEEP (*(.eh_frame))
	} >FLASH

	/* .ARM.exidx is sorted, so has to go in its own output section. */
	PROVIDE_HIDDEN (__exidx_start = .);
	.ARM.exidx :
	{
	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >FLASH
	PROVIDE_HIDDEN (__exidx_end = .);

	. = ALIGN(4);
	/* nemui - end */
    
    _etext = . ;
    PROVIDE (etext = .);
	/* This is used by the startup in order to initialize the .data secion */
    _sidata = _etext;


    
    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data  : AT ( _sidata )
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data secion */
        _sdata = . ;
        _data = .;
        *(.data)
        *(.data.*)
		KEEP(*(.jcr))
		*(.got.plt) *(.got)
		*(.shdata)
		*(.gnu.linkonce.d*)
		
        . = ALIGN(4); 	/* 20090714 Nemui */
        *(.ramfunc*) 	/* 20090714 Nemui */
        . = ALIGN(4); 	/* 20090714 Nemui */
		
        *(.RAMtext)
	    . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .data secion */
   	 _edata = . ;
    } >RAM0	

    /* This is the uninitialized data section */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;
        __bss_start = . ;
        __bss_start__ = . ;
        *(.bss) 
        *(.bss.*) /* mthomas 7/2006 */
        *(.gnu.linkonce.b*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
    } > RAM1
    __bss_end__ = . ;
    PROVIDE (__bss_end = .);
    /* This is the heap section used malloc */
	.heap (NOLOAD) :
	{
		_end = . ;
		end = . ;
		PROVIDE (_end = .);
		PROVIDE (end = .);
		PROVIDE (_heap = .);
		*(.heap)
		*(.HEAP)
		. = ALIGN(4);
	} > RAM1
	. = ALIGN(4);
	_heap_end = . ;
	PROVIDE (_heap_end = .);


	/* after that it's only debugging information. */

	/* remove the debugging information from the standard libraries */
	/DISCARD/ :
	{
	 libc.a ( * )
	 libm.a ( * )
	 libgcc.a ( * )
	 }

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
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
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }

	.note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
	.ARM.attributes 	0 : { KEEP (*(.ARM.attributes)) }
	/DISCARD/ : { *(.note.GNU-stack) }
	/DISCARD/ : { *(.gnu.lto*) }
}
