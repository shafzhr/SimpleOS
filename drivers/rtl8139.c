/**
 * @brief A driver for the RTL8139 NIC
 * 
 */
#include "rtl8139.h"
#include "screen.h"
#include "ports.h"
#include "../cpu/pci.h"
#include "../cpu/isr.h"
#include "../utils/string.h"
#include "../utils/utils.h"
#include <stdint.h>
#include <stdbool.h>

static pci_device_t device = { 0 };

static uint8_t rx_buffer[RTL8139_RX_SIZE];
static uint8_t tx_buffer[RTL8139_TX_SIZE];
static uint8_t mac_addr[6];


static void rtl8139_handler(registers_t* regs);

bool rtl8139_init()
{
    kprint("[RTL8139] Initializing\n");

    device = pciGetDevice(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID);
    if (device.address == 0)
    {
        kprint("[RTL8139] Initialization failed\n");
        return false;
    }

    uint32_t ioaddr = device.bar0 & 0xFFFFFFFC;
    
    // PCI Bus Mastering
    uint32_t pci_command_register = pciRead(device.address, PCI_COMMAND);
    if (!(pci_command_register & (1 << 2))) // check bit 2
    {
        pci_command_register |= (1 << 2);   // set bit 2
        pciWrite(device.address, PCI_COMMAND, pci_command_register); 
    }
    
    outb(ioaddr + 0x52, 0x0);                       // turn on the rtl8139
    outb(ioaddr + 0x37, 0x10);                      // software reset
    while ( (inb(ioaddr + 0x37) & 0x10) != 0) { }   // wait for reset to complete

    outl(ioaddr + 0x30, (uintptr_t)rx_buffer);      // init recieve buffer
    memset(rx_buffer, 0, RTL8139_RX_SIZE);
    
    outw(ioaddr + 0xC, 0x0005);                     // sets the TOK and ROK bits high
    
    outl(ioaddr + 0x44, 0xf | (1 << 7));        // configure recieve buffer
                                                    // (1 << 7) is the WRAP bit
                                                    // 0xf is AB+AM+APM+AAP
    
    outb(ioaddr + 0x37, 0x0C);                  // enables recieve and transmit
    
    uint8_t irq_num = (uint8_t)(pciRead(device.address, PCI_INTERRUPT_LINE) & 0xFF);
    register_isr_handler(0x20 + irq_num, rtl8139_handler);
    
    for (size_t i = 0; i < 6; ++i)
    {
        mac_addr[i] = inb(ioaddr + RTL8139_MAC_OFFSET + i);
    }
    char mac_byte_hex[3];
    kprint("[RTL8139] MAC address is: ");
    kprint(itoa(mac_addr[0], mac_byte_hex, 16));    put_char(':'); 
    kprint(itoa(mac_addr[1], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[2], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[3], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[4], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[5], mac_byte_hex, 16));
    put_char('\n');


    return true;
}

static void rtl8139_handler(registers_t* regs)
{

}