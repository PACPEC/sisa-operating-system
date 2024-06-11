__attribute__((noreturn)) void umain() {
  char * vga = 0xA000;
  *vga = 'B';
  for (;;);
}