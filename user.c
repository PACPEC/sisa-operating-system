int tics_timer, tecla_pulsada;

void print(char *str, char *vga) {
    int i;
    for(i = 0; i < sizeof(str); i++) {
        vga[i<<1] = str[i];
    }
}

__attribute__((noreturn)) void umain() {
    char * vga = 0xA000;
    char str[16] = {'h', 'e', 'l', 'l', 'o', '\n'};
    print(str, vga);
    for (;;);
}
