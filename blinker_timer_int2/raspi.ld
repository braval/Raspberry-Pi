
MEMORY
{
   ram : ORIGIN = 0x0000, LENGTH = 0x1000
}

SECTIONS
{
   .text : {
      *(.text)
   } > ram
   text_size =SIZEOF(.text);
   .data : { *(.data) } > ram
   __bss_start__ = .;
   .bss : { *(.bss) } > ram
   __bss_end__ = .;   
}
