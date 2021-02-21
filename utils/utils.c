#include "string.h"
#include "utils.h"

char* itoa(int value, char* buffer, int base)
{
    int n = abs(value);
    int index = 0;
    while (n)
    {
        int digit = n % base;
        if (digit >= 10)
        {
            buffer[index++] = 0x41 + digit - 10;         
        }
        else
        {
            buffer[index++] = 0x30 + digit;
        }
        n /= base;
        
    }
    if (value == 0)
    {
        buffer[index++] = '0';
    }
    if (value < 0 && base == 10)
    {
        buffer[index++] = '-';
    }

    buffer[index] = '\0';
    
    return strrev(buffer);
}

int abs(int n)
{
    int nums[2] = {-n, n};
    return nums[n > 0];
}
