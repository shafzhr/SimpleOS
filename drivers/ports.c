/**
 * @file ports.c
 * 
 * Ports I/O.
 */
#include <stdint.h>



/**
 * @brief Read a byte from a port
 * 
 * @param port Port nubmer
 * @return a read byte
 */
uint8_t port_in(uint16_t port)
{
    unsigned char result;
    __asm__("in %%al, %%dx": "=a"(result) : "d"(port));
    return result;
}

/**
 * @brief Write a byte to a port
 * 
 * @param port Port nuber
 * @param data A byte to write
 */
void port_out(uint16_t port, uint8_t data)
{

    __asm__("out %%dx, %%al" : : "d"(port), "a"(data));
}

/**
 * @brief Read a word from a port
 * 
 * @param port Port number
 * @return a read word
 */
uint16_t word_port_in(uint16_t port)
{
    unsigned char result;
    __asm__("in %%ax, %%dx": "=a"(result) : "d"(port));
    return result;

}

/**
 * @brief Write a word to a port
 * 
 * @param port Port number
 * @param data A word to write
 */
void word_port_out(uint16_t port, uint16_t data)
{

    __asm__("out %%dx, %%ax" : : "d"(port), "a"(data));
}

