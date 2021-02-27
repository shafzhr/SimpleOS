#include "../drivers/ports.h"
#include <stdint.h>


/**
 * @brief Reads 16-bit from the configuration space.
 *        https://wiki.osdev.org/PCI#Configuration_Space_Access_Mechanism_.231
 * 
 * @param bus Bus number
 * @param slot Device number
 * @param func Function number
 * @param offset Register offset - has to point to consecutive DWORDs -> bits 0 ant 1 are always 0.
 * @return uint16_t 
 */
uint16_t pciRead(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
 
    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
 
    /* write out the address */
    outl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return tmp;
}

uint16_t pciWrite(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data)
{
    
}
