OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

/* Define program header segments */
PHDRS {
    code_seg PT_LOAD;
    data_seg PT_LOAD;
}

/* Define sections and their memory mappings */
SECTIONS {
    /* Code segment */
    .text : {
        /* The first symbol in the .text section */
        *(.text.start)
        /* All other symbols in the .text section */
        *(.text*)
    } : code_seg

    /* Read-only data segment */
    .rodata : {
       /* Read-only data symbols */
        *(.rodata)
        *(.rodata*)
    } : code_seg

    /* Data segment */
    .data :{
        /* Initialized data symbols */
        *(.data)
    } : data_seg

    /* BSS segment - Uninitialized data */
    .bss : {
        *(.bss)
    } : data_seg
}
