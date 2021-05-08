#include "ethernet.h"
#include "../utils/string.h"
#include <stdint.h>


ethernet_header_t ethernet_read_header(uint8_t* data)
{
    ethernet_header_t hdr = { 0 };
    memcpy(&hdr, data, sizeof(ethernet_header_t));
    return hdr;
}
