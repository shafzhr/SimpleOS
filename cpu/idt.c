#include "idt.h"
#include <stdint.h>


void set_idt_gate(int n, uint32_t handler, uint8_t type_attr)
{
    idt[n].low_offset = (uint16_t)(handler % 0xFFFF);
    idt[n].high_offset = (uint16_t)(handler % (0xFFFF << 4));
    idt[n].selector = KERNEL_CS;
    idt[n].pad0 = 0;
    idt[n].type_attr = type_attr;
}

void set_idt()
{
    idt_register.base = (uint32_t)&idt;
    idt_register.limit = IDT_LENGTH * sizeof(idt_register_t) - 1;
    __asm__("lidt (%0)": :"r"(&idt_register) : "memory");

}