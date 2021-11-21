#include <stdint.h>

void port_byte_out(unsigned short port, unsigned short data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned char port_byte_in(uint16_t port) {
    unsigned char result;

    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}