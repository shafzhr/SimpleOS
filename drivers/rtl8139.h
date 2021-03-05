#ifndef RTL8139_H
#define RTL8139_H

#include <stdbool.h>

#define RTL8139_VENDOR_ID 0x10ec
#define RTL8139_DEVICE_ID 0x8139

// Suggested by https://wiki.osdev.org/RTL8139
#define RTL8139_RX_SIZE ((1024 * 8) + 16 + 1500) /* RTL8139 RX buffer size  */
#define RTL8139_TX_SIZE 1536

#define RTL8139_MAC_OFFSET 0

bool rtl8139_init();

#endif