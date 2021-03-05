#include "pci.h"
#include "../drivers/ports.h"
#include "../drivers/screen.h"
#include "../utils/utils.h"
#include <stdint.h>



static uint32_t pciGetAddress(uint8_t bus, uint8_t slot, uint8_t func)
{
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    return (lbus << 16) | (lslot << 11) | (lfunc << 8) | PCI_ENABLE_BIT;   
}

/**
 * @brief Reads 32-bit from the configuration space.
 *        https://wiki.osdev.org/PCI#Configuration_Space_Access_Mechanism_.231
 * 
 * @param address device's configuration address
 * @param offset Register offset - has to point to consecutive DWORDs -> bits 0 ant 1 are always 0.
 * @return uint32_t read data
 */
uint32_t pciRead(uint32_t address, uint8_t offset)
{
    /* write out the address */
    outl(PCI_CONFIG_ADDR_PORT, address | (offset & 0xFC));
    /* read in the data */
    return inl(PCI_CONFIG_DATA_PORT);
}

void pciWrite(uint32_t address, uint8_t offset, uint32_t data)
{
    outl(PCI_CONFIG_ADDR_PORT, address | (offset & 0xFC));
    outl(PCI_CONFIG_DATA_PORT, data);
}

uint16_t pciGetVendorId(uint32_t address)
{
    uint32_t vendor_id = pciRead(address, 0) & 0xFFFF;
    return vendor_id;
}

uint16_t pciGetDeviceId(uint32_t address)
{
    uint32_t device_id = (pciRead(address, 0) >> 16) & 0xFFFF;
    return device_id;
}

/**
 * @brief gets device by vendor id and device id
 *        TODO: - Add support for multiple devices with the same vendor id and device id.
 *              - Add support for functions.
 * 
 * @param vendor_id vendor id
 * @param device_id device id
 * @return pci_device_t
 */
pci_device_t pciGetDevice(uint16_t vendor_id, uint16_t device_id)
{
    for (int bus = 0; bus < 256; ++bus)
    {
        for (uint8_t slot = 0; slot < 32; ++slot)
        {
            for (uint8_t func = 0; func < 1; ++func)
            {
                uint32_t address = pciGetAddress(bus, slot, func);
                uint16_t v_id = pciGetVendorId(address);
                if (v_id == PCI_NON_EXISTENT)
                {
                    continue;
                }
                uint16_t d_id = pciGetDeviceId(address);
                if (vendor_id == v_id && device_id == d_id)
                {
                    char id_buffer[20];
                    kprint("Found a PCI device: ");
                    kprint(itoa(v_id, id_buffer, 16));
                    kprint(" | ");
                    kprint(itoa(d_id, id_buffer, 16));
                    kprint("\n");

                    pci_device_t device = { 0 };
                    device.address = address;
                    device.vendor_id = vendor_id;
                    device.device_id = device_id;
                    device.bar0 = pciRead(address, PCI_BAR0);
                    return device;
                }
            }
        }
    }
    return (pci_device_t){ 0 };
    
}
