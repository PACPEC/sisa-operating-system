ENTRY(__entry_point)
OUTPUT_FORMAT("elf32-sisa","elf32-sisa","elf32-sisa")
OUTPUT_ARCH(sisa)
SECTIONS
{
  . = 0x0000;

  .usercode : {
    PROVIDE (__user_start = .);
    wrappers.o(.text)
    programs/*.o(.text)
    wrappers.o(.rodata)
    programs/*.o(.rodata)
    . = ALIGN(2);
  }

  . = 0x4000;

  .userdata : {
    PROVIDE(user_stack_top = .);
    . += 0x2000;
    PROVIDE(user_stack_bottom = .);
    wrappers.o(.data)
  }

  . = 0xc000;

  .system : {
    *(.text)
    *(.rodata)
    . = ALIGN(2);
  }

  __entry_point = LOADADDR(.system);
  
  . = 0x8000 ;

  .sysdata : {
    PROVIDE(system_stack_top = .);
    . += 4096;
    PROVIDE(system_stack_bottom = .);
    *(.data)
  }
}
