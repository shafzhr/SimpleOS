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
#include "../utils/mem.h"
#include "../net/net.h"
#include "../net/ipv4.h"
#include "../net/ethernet.h"
#include <stdint.h>
#include <stdbool.h>

#define UNUSED(x) (void)(x)

static pci_device_t device[2] = { 0 };
static uint32_t ioaddr[2];

static uint8_t* tx_buffer[2];
// static uint8_t padding[RTL8139_RX_SIZE]; // -1771
// static uint8_t rx_buffer[2][RTL8139_RX_SIZE];
static uint8_t* rx_buffer[2];
static uint32_t rx_index[2] = { 0 };
static uint8_t tx_counter[2] = { 0 };

static uint8_t mac_addr[2][6];


isr_handler_t handler_picker(uint8_t device_index);
static void rtl8139_handler(registers_t* regs, uint8_t device_index);
static void rtl8139_handler0(registers_t* regs);
static void rtl8139_handler1(registers_t* regs);
static void rtl8139_wrong_handler(registers_t* regs);

static void rtl8139_receive_packet(uint8_t device_index);
bool rtl8139_init_device(int device_index);

bool rtl8139_init()
{
    kprint("[RTL8139] Initializing\n");
    bool status1 = rtl8139_init_device(0); 
    bool status2 = rtl8139_init_device(1);
    return status1 && status2;
}

bool rtl8139_init_device(int device_index)
{    

    kprint("[RTL8139] Initializing\n");

    device[device_index] = pciGetDevice(RTL8139_VENDOR_ID, RTL8139_DEVICE_ID, device_index);
    
    if (device[device_index].address == 0)
    {
        kprint("[RTL8139] Initialization failed\n");
        return false;
    }
    ioaddr[device_index] = device[device_index].bar0 & 0xFFFFFFFC;
    // PCI Bus Mastering
    uint32_t pci_command_register = pciRead(device[device_index].address, PCI_COMMAND);
    if (!(pci_command_register & (1 << 2))) // check bit 2
    {
        pci_command_register |= (1 << 2);   // set bit 2
        pciWrite(device[device_index].address, PCI_COMMAND, pci_command_register); 
    }
    
    outb(ioaddr[device_index] + 0x52, 0x0);                                     // turn on the rtl8139
    outb(ioaddr[device_index] + 0x37, 0x10);                                    // software reset
    while ( (inb(ioaddr[device_index] + 0x37) & 0x10) != 0) { }                 // wait for reset to complete

    rx_buffer[device_index] = (uint8_t*)kmalloc(RTL8139_RX_SIZE, 1);
    outl(ioaddr[device_index] + 0x30, (uintptr_t)rx_buffer[device_index]);      // init receive buffer
    memset(rx_buffer[device_index], 0, RTL8139_RX_SIZE);
    
    tx_buffer[device_index] = (uint8_t*)kmalloc(RTL8139_TX_SIZE, 0);

    outw(ioaddr[device_index] + 0x3C, 0x0005);                                  // sets the TOK and ROK bits high
    
    outl(ioaddr[device_index] + 0x44, 0xf | (1 << 7));                          // configure receive buffer
                                                                                // (1 << 7) is the WRAP bit
                                                                                // 0xf is AB+AM+APM+AAP
    
    outb(ioaddr[device_index] + 0x37, 0x0C);                                    // enables receive and transmit
    
    // char addrbuf[10];
    // kprint("[RTL8139] Address -> ");
    // kprint(itoa(device[device_index].address, addrbuf, 16));
    // kprint("\n");

    uint8_t irq_num = (uint8_t)(pciRead(device[device_index].address, PCI_INTERRUPT_LINE) & 0xFF);
    isr_handler_t handler = handler_picker(device_index);
    if (handler == rtl8139_wrong_handler)
    {
        kprint("[RTL8139] Wrong handler chosen\n");
        return false;
    }
    register_isr_handler(0x20 + irq_num, handler);
    
    kprint("[RTL8139] Registered at #");
    char irq_number_str[4];
    kprint(itoa(irq_num+0x20, irq_number_str, 16));
    put_char('\n');
    // kprint("[RTL8139] rx_buffer -> ");
    // kprint(itoa((uint32_t)rx_buffer[device_index], addrbuf, 16));
    // put_char('\n');
    

    for (size_t i = 0; i < 6; ++i)
    {
        mac_addr[device_index][i] = inb(ioaddr[device_index] + RTL8139_MAC_OFFSET + i);
    }
    char mac_byte_hex[3];
    kprint("[RTL8139] MAC address is: ");
    kprint(itoa(mac_addr[device_index][0], mac_byte_hex, 16));    put_char(':'); 
    kprint(itoa(mac_addr[device_index][1], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[device_index][2], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[device_index][3], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[device_index][4], mac_byte_hex, 16));    put_char(':');
    kprint(itoa(mac_addr[device_index][5], mac_byte_hex, 16));
    put_char('\n');


    return true;
}

isr_handler_t handler_picker(uint8_t device_index)
{
    switch (device_index)
    {
    case 0:
        return rtl8139_handler0;
    case 1:
        return rtl8139_handler1;
    default:
        return rtl8139_wrong_handler;
    }
}

static void rtl8139_wrong_handler(registers_t* regs)
{
    UNUSED(regs);
}

static void rtl8139_handler0(registers_t* regs)
{
    UNUSED(regs);
    rtl8139_handler(regs, 0);
}

static void rtl8139_handler1(registers_t* regs)
{
    UNUSED(regs);
    rtl8139_handler(regs, 1);
}

static void rtl8139_handler(registers_t* regs, uint8_t device_index)
{
    UNUSED(regs);
    while (true)
    {
        uint16_t status = inw(ioaddr[device_index] + RTL8139_ISR_OFFSET);
        outw(ioaddr[device_index] + RTL8139_ISR_OFFSET, status); // clear interrupt
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
            rtl8139_receive_packet(device_index);
        }
    }  
}

void rtl8139_send_packet(void* packet, uint32_t size, uint8_t device_index)
{
    uint8_t cnt = tx_counter[device_index];
    memcpy(tx_buffer[device_index], packet, size);
    
    outl(ioaddr[device_index] + 0x20 + cnt * 4, (uint32_t)tx_buffer[device_index]);
    outl(ioaddr[device_index] + 0x10 + cnt * 4, size);

    tx_counter[device_index] = (cnt >= 3) ? 0 : cnt + 1;
}

static void rtl8139_receive_packet(uint8_t device_index)
{
    kprint("Landed receive\n");
    uint32_t index = rx_index[device_index];
    while ((inb(ioaddr[device_index] + RTL8139_COMMAND) & 0x1) == 0)  // 0 means that the buffer is empty
    {
        uint32_t offset = index % RTL8139_RX_SIZE;
        uint32_t read_ptr = (uint32_t)rx_buffer[device_index] + offset;
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
            uint8_t transmit_buf[RTL8139_TX_SIZE];
            uint8_t* buf = (uint8_t*)(read_ptr + sizeof(rtl8139_header));

            ethernet_header_t eth_header = ethernet_read_header(buf);
            buf += sizeof(ethernet_header_t);
            if (swap_uint16(eth_header.type) == ETHERTYPE_IPV4)
            {
                kprint("[Ethernet] Recieved a packet to transfer\n");
                ipv4_header_t ipv4_header = ipv4_read_header(buf);
                buf += sizeof(ipv4_header); // now buffer points to data/ip options/padding
                if (ipv4_header.version == 4)
                {
                    kprint("[IPv4] Recieved a packet to transfer\n");
                    uint8_t other_NIC = device_index ^ 1;
                    memcpy(&eth_header.src, mac_addr[other_NIC], 6); // Source = other NIC
                    memset(&eth_header.dst, 0xFF, 6); // Dest = broadcast
                    ipv4_header.ttl = 0x50;
                    
                    // Copy data to transmit buffer
                    uint8_t* tbuf_ptr = transmit_buf;
                    memcpy(tbuf_ptr, &eth_header, sizeof(ethernet_header_t));
                    tbuf_ptr += sizeof(ethernet_header_t);
                    memcpy(tbuf_ptr, &ipv4_header, sizeof(ipv4_header_t));
                    tbuf_ptr += sizeof(ipv4_header_t);
                    uint16_t total_len = swap_uint16(ipv4_header.len);
                    uint8_t ipv4_left_to_read = total_len - 0x20;   // 0x20 is the minimum header size (what we've already read until now)
                    memcpy(tbuf_ptr, buf, ipv4_left_to_read);       // "options" ipv4 header field
                    rtl8139_send_packet(transmit_buf, 0x14 + total_len, other_NIC);
                }
            }
        }
        index = (index + rx_header_ptr->size + sizeof(rtl8139_header) + 3) & 0xFFFFFFFC;
        outw(ioaddr[device_index] + RTL8139_RX_BUF_PTR, index - 0x10);

    }
    rx_index[device_index] = index;
}