#ifndef H_PCI
#define H_PCI

#include <stdint.h>

uint16_t pciRead(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

#endif