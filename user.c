#include "wrappers.h"

char *vga = (char *)0xA000;

void printu(char *str) {
    while(*str != '\0' && *str != '\n') {
        *vga = *(str++);
        vga += 2;
    }
    vga = ' ';
    vga += 2;
}

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str) {
    int i = 0;
    int isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    while (num != 0) {
        int rem = num % 10;
        str[i++] = rem + '0';
        num = num / 10;
    }
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

__attribute__((noreturn)) void umain() {
    int t;
    char buff[32];
    itoa(&t, buff);
    printu(buff);
    getTicks(&t);
    for (;;);
}
