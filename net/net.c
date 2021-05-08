#include <stdint.h>

/**
 * @brief bswap for 16-bit unsigned.
 */
uint16_t swap_uint16(uint16_t val)
{
    return (val << 8) | (val >> 8 );
}

/**
 * @brief bswap for 16-bit signed.
 */
int16_t swap_int16(int16_t val)
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

/**
 * @brief bswap for 32-bit unsigned.
 */
uint32_t swap_uint32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

/**
 * @brief bswap for 32-bit signed.
 */
int32_t swap_int32(int32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}