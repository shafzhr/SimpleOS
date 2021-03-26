#include <stdint.h>


uint32_t free_mem_addr = 0x1010000;

uint32_t kmalloc(uint32_t size, uint8_t align)
{
    if (align == 1)
    {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    uint32_t return_addr = free_mem_addr;
    free_mem_addr += size;
    return return_addr;
}


