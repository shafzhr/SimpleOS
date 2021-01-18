/**
 * @file ports.c
 * 
 * Ports I/O.
 */


/**
 * @brief Read a byte from a port
 * 
 * @param port Port nubmer
 * @return a read byte
 */
unsigned char port_in(unsigned short port)
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
void port_out(unsigned short port, unsigned char data)
{

    __asm__("out %%dx, %%al" : : "d"(port), "a"(data));
}

/**
 * @brief Read a word from a port
 * 
 * @param port Port number
 * @return a read word
 */
unsigned short word_port_in(unsigned short port)
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
void word_port_out(unsigned short port, unsigned short data)
{

    __asm__("out %%dx, %%ax" : : "d"(port), "a"(data));
}

