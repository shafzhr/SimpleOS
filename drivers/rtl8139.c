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

#define UNUSED(x) (void)(x)

static pci_device_t device = { 0 };
static uint32_t ioaddr;

static uint8_t rx_buffer[RTL8139_RX_SIZE];
static uint8_t tx_buffer[4][RTL8139_TX_SIZE];
static uint32_t rx_index = 0;
static uint8_t tx_counter = 0;

static uint8_t mac_addr[6];


static void rtl8139_handler(registers_t* regs);
static void rtl8139_receive_packet();

bool rtl8139_init()
{
    kprint("[RTL8139] Initializing\n");

    device = pciGetDevice(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID);
    if (device.address == 0)
    {
        kprint("[RTL8139] Initialization failed\n");
        return false;
    }

    ioaddr = device.bar0 & 0xFFFFFFFC;
    
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

    outl(ioaddr + 0x30, (uintptr_t)rx_buffer);      // init receive buffer
    memset(rx_buffer, 0, RTL8139_RX_SIZE);
    
    outw(ioaddr + 0x3C, 0x0005);                     // sets the TOK and ROK bits high
    
    outl(ioaddr + 0x44, 0xf | (1 << 7));        // configure receive buffer
                                                    // (1 << 7) is the WRAP bit
                                                    // 0xf is AB+AM+APM+AAP
    
    outb(ioaddr + 0x37, 0x0C);                  // enables receive and transmit
    
    uint8_t irq_num = (uint8_t)(pciRead(device.address, PCI_INTERRUPT_LINE) & 0xFF);
    register_isr_handler(0x20 + irq_num, rtl8139_handler);
    
    // kprint("[RTL8139] Registered at #");
    // char irq_number_str[4];
    // kprint(itoa(irq_num+0x20, irq_number_str, 10));
    // put_char('\n');


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
    UNUSED(regs);
    while (true)
    {
        uint16_t status = inw(ioaddr + RTL8139_ISR_OFFSET);
        outw(ioaddr + RTL8139_ISR_OFFSET, status); // clear interrupt
        if (status == 0)
        {
            break;
        }
        if (status & RTL8139_TOK) /* Transmit OK */
        {
            // Log sent
        }
        if (status & RTL8139_ROK) /* Recieve OK */
        {
            // Log received
            rtl8139_receive_packet();
        }
    }  
}

void rtl8139_send_packet(void* packet, uint32_t size)
{
    memcpy(tx_buffer[tx_counter], packet, size);
    
    outl(ioaddr + 0x20 + tx_counter * 4, (uint32_t)tx_buffer);
    outl(ioaddr + 0x10 + tx_counter * 4, size);

    tx_counter = (tx_counter >= 3) ? 0 : tx_counter + 1;
}

static void rtl8139_receive_packet()
{
    uint32_t index = rx_index;
    while ((inb(ioaddr + RTL8139_COMMAND) & 0x1) == 0)  // 0 means that the buffer is empty
    {
        uint32_t offset = index % RTL8139_RX_SIZE;
        uint32_t read_ptr = (uint32_t)rx_buffer + offset;
        rtl8139_header* rx_header_ptr =  (rtl8139_header*)read_ptr;
        

        if (rx_header_ptr->status & (
                                    RTL8139_RX_PACKET_HEADER_FAE  | /* Frame  Alignment  Error */
                                    RTL8139_RX_PACKET_HEADER_CRC  | /* CRC  Error */
                                    RTL8139_RX_PACKET_HEADER_RUNT | /* Runt  Packet  Received (Length < 64) */
                                    RTL8139_RX_PACKET_HEADER_LONG   /* Long  Packet (Length > 4k) */
                                    )
            )
        {
            kprint("[RTL8139] ERR: rx packet header error\n");
        }
        else
        {
            char hex_buf[3];
            uint8_t* buf = (uint8_t*)(read_ptr + sizeof(rtl8139_header));
            // TODO: Handle payload
            if (*buf == mac_addr[0]) // TODO: make it use full comparison
            {
                for (size_t i = 0; i < rx_header_ptr->size; ++i)
                {
                    uint8_t val = *(buf+i);
                    kprint(itoa(val, hex_buf, 16));
                    put_char(' ');
                }
                put_char('\n');
            }                        
        }
        index = (index + rx_header_ptr->size + sizeof(rtl8139_header) + 3) & 0xFFFFFFFC;
        outw(ioaddr + RTL8139_RX_BUF_PTR, index - 0x10);

    }
    rx_index = index;
}