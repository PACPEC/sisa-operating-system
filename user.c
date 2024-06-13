int tics_timer;
char tecla_pulsada;

void print(char *str, char *vga) {
    while(*str!='\n' && *str!='\0') {
        *vga = *str;
        str++;
        vga += 2;
    }
}

__attribute__((noreturn)) void umain() {
    
    for (;;);
}
