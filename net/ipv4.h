#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>

typedef struct
{
    uint8_t ihl: 4;     // switched ihl and version beacuse of little endianness
    uint8_t version: 4; // ^
    uint8_t service_type;
    uint16_t len;
    uint16_t identification;
    uint16_t fragment_offset: 13;   // little endian
    uint8_t flags: 3;               // ^
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src;
    uint32_t dst;
} __attribute__((packed)) ipv4_header_t;

ipv4_header_t ipv4_read_header(uint8_t* data);

#endif
