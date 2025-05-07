#include "config.h"

#include "subsystems/uart.h"

size_t Config::readString(char* str, size_t maxSize) {
    size_t length = 0;
    while (true) {
        fflush(stdout);
        char c; UART::read((uint8_t*)&c, 1); // get a character from serial console
        switch (c) {
            case '\n': // LF - might be sent as CRLF, ignore
                break;
            case '\r': // CR - RETURN pressed
                str[length] = '\0';
                printf("\r\n");
                return length;
            case '\b': // backspace
                if (length > 0) {
                    length--;
                    printf("\b \b"); // clear character from serial console
                }
                break;
            default:
                if (length < maxSize - 1) { // there is still space to store the character
                    str[length++] = c;
                    putchar(c);
                }
                break;
        }
    }
}

bool Config::readYN() {
    fflush(stdout);
    while (true) {
        char c; UART::read((uint8_t*)&c, 1);
        switch (c) {
            case 'y': printf("%c\r\n", c); return true;
            case 'Y': printf("%c\r\n", c); return true;
            case 'n': printf("%c\r\n", c); return false;
            case 'N': printf("%c\r\n", c); return false;
            default: break;
        }
    }
}
