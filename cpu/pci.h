#ifndef H_PCI
#define H_PCI

#include <stdint.h>

#define PCI_CONFIG_ADDR_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC
#define PCI_NON_EXISTENT 0xFFFF

#define PCI_BAR0 0x10

typedef struct
{
    uint32_t address;
    uint32_t device_id, vendor_id;
    uint32_t bar0, bar1, bar2, bar3, bar4, bar5;

} pci_device_t;


uint32_t pciRead(uint32_t address, uint8_t offset);
void pciWrite(uint32_t address, uint8_t offset, uint32_t data);
pci_device_t pciGetDevice(uint16_t vendor_id, uint16_t device_id);

#endif