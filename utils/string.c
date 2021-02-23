#include "string.h"
#include <stddef.h>


void chrswap(char* x, char* y)
{
    char tmp = *x;
    *x = *y;
    *y = tmp;
}

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

char* strrev(char* buffer)
{
    char* str = buffer;

    size_t begin = 0;
    size_t end = strlen(buffer) - 1;
    while (begin < end)
    {
        chrswap(&str[begin++], &str[end--]);
    }
    return buffer;
}

size_t strlen(char* str)
{
    size_t len = 0;
    while (str[len]) { ++len; }
    return len;
    
}