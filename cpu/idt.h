#ifndef H_IDT
#define H_IDT

#include <stdint.h>

#define IDT_LENGTH 256
#define KERNEL_CS 0x8

typedef struct
{
    uint16_t low_offset;
    uint16_t selector; // code-sement selector
    uint8_t pad0;
    uint8_t type_attr;
    uint16_t high_offset;
} __attribute__((packed)) IDTDescr;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

extern IDTDescr idt[IDT_LENGTH];
extern idt_register_t idt_register;

void set_idt_gate(int n, uint32_t handler, uint8_t type_attr);
void set_idt();

#endif