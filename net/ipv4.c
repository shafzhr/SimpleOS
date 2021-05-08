#include "ipv4.h"
#include "../utils/string.h"
#include <stdint.h>


ipv4_header_t ipv4_read_header(uint8_t* data)
{
    ipv4_header_t hdr = { 0 };
    memcpy(&hdr, data, sizeof(ipv4_header_t));
    return hdr;
}
