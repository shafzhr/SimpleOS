#ifndef H_PORTS
#define H_PORTS

#include <stdint.h>

uint8_t port_in(uint16_t port);
void port_out(uint16_t port, uint8_t data);
uint16_t word_port_in(uint16_t port);
void word_port_out(uint16_t port, uint16_t data);

#endif