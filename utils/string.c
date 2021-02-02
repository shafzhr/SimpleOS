#include "string.h"
#include <stddef.h>

/**
 * @brief Copies "len" bytes from "src" to "dst"
 * 
 * @todo Optimize using 16-bit and 32-bit copies
 * @param dst destination
 * @param src source
 * @param len amount of bytes to copy
 * @return void* dst, in order to allow chaining
 */
void* memcpy(void* dst, void* src, size_t len)
{
    char* dp = dst;
    const char* sp = src;
    for (size_t i = 0; i < len; ++i)
    {
        dp[i] = sp[i];
    }
    return dst;    
}
