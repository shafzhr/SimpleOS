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
uint8_t inb(uint16_t port)
{
    uint8_t result = 0;
    #ifdef ASMINTEL
        #pragma message "using INTEL"
        asm volatile("in %0, %1": "=a"(result) : "Nd"(port));
    #else
        #pragma message "using ATT"
        asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    #endif
    return result;
}

/**
 * @brief Write a byte to a port
 * 
 * @param port Port nuber
 * @param data A byte to write
 */
void outb(uint16_t port, uint8_t data)
{
    #ifdef ASMINTEL
        asm volatile("out %%dx, %%al" : : "d"(port), "a"(data));
    #else
        asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
    #endif
}

/**
 * @brief Read a word from a port
 * 
 * @param port Port number
 * @return a read word
 */
uint16_t inw(uint16_t port)
{
    uint16_t result = 0;
    #ifdef ASMINTEL
        asm volatile("in %%ax, %%dx": "=a"(result) : "d"(port));
    #else
        asm volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    #endif
    return result;

}

/**
 * @brief Write a word to a port
 * 
 * @param port Port number
 * @param data A word to write
 */
void outw(uint16_t port, uint16_t data)
{
    #ifdef ASMINTEL
        asm volatile("out %%dx, %%ax" : : "d"(port), "a"(data));
    #else
        asm volatile("outw %0, %1" : : "a"(data), "Nd"(port));
    #endif
}

/**
 * @brief Read a dword from a port
 * 
 * @param port Port number
 * @return a read dword
 */
uint32_t inl(uint16_t port)
{
    uint32_t result = 0;
    #ifdef ASMINTEL
        asm volatile("in %0, %1": "=a"(result) : "Nd"(port));
        // asm volatile("in %%eax, %%dx": "=a"(result) : "d"(port));
    #else
        asm volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
    #endif
    return result;

}

/**
 * @brief Write a dword to a port
 * 
 * @param port Port number
 * @param data A dword to write
 */
void outl(uint16_t port, uint32_t data)
{
    #ifdef ASMINTEL
        asm volatile("out %1, %0" : : "a"(data), "Nd"(port));
        // asm volatile("out %%dx, %%eax" : : "d"(port), "a"(data));
    #else
        asm volatile("outl %0, %1" : : "a"(data), "Nd"(port));
    #endif
}
