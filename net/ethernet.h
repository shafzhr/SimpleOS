#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>

#define ETHERTYPE_IPV4 0x0800
#define ARP 0x0806

typedef struct
{
    uint8_t dst[6];
    uint8_t src[6];
    uint16_t type;  
} __attribute__((packed)) ethernet_header_t;


ethernet_header_t ethernet_read_header(uint8_t* data);

#endif // ETHERNET_H

