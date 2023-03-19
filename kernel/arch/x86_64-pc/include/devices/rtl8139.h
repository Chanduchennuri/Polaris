#ifndef RTL8139_H
#define RTL8139_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139

#define RTL8139_REG_MAC05 0x00
#define RTL8139_REG_MAC07 0x04
#define RTL8139_REG_RBSTART 0x30
#define RTL8139_REG_CMD 0x37
#define RTL8139_REG_IMR 0x3c
#define RTL8139_REG_ISR 0x3e

#define RX_BUFFER_SIZE 8192

bool rtl8139_init(void);

#endif