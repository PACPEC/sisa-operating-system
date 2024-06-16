ENTRY(__entry_point)
OUTPUT_FORMAT("elf32-sisa","elf32-sisa","elf32-sisa")
OUTPUT_ARCH(sisa)
SECTIONS
{
  . = 0x0000;

  .usercode : {
    PROVIDE (__user_start = .);
    user.o(.text)
    wrappers.o(.text)
    programs/*.o(.text)
    user.o(.rodata)
    wrappers.o(.rodata)
    programs/*.o(.rodata)
    . = ALIGN(2);
  }

  . = 0x4000;

  .userdata : {
    PROVIDE(user_stack_top = .);
    . += 4096;
    PROVIDE(user_stack_bottom = .);
    user.o(.data)
    wrappers.o(.data)
  }

  __user_end = __user_start + SIZEOF(.user);

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
