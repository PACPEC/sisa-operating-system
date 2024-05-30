ENTRY(__entry_point)
OUTPUT_FORMAT("elf32-sisa","elf32-sisa","elf32-sisa")
OUTPUT_ARCH(sisa)
SECTIONS
{
  .user : {
    . = 0x000;
    PROVIDE (__user_start = .);
  }

  __user_end = __user_start + SIZEOF(.user);

  .system : {
    . = 0xc000 ;
    *(.text)
    *(.rodata)
    . = ALIGN(2);
  }

  __entry_point = LOADADDR(.system);
  
  .sysdata : {
    . = 0x8000 ;
    PROVIDE(system_stack_top = .);
    . += 4096;
    PROVIDE(system_stack_bottom = .);
    *(.data)
  }
}
