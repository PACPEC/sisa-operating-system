__attribute__((noreturn)) void umain();

__attribute__((noreturn)) void kmain() {
  char * vga = 0xA000;
  *vga = 'A';
  umain();
  for (;;);
}
