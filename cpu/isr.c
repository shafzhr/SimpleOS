#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include <stdint.h>


/**
 * @brief installs the ISRs.
 * note that 0x8F attribute is for traps and 0x8E is for interrupts (hardware)
 * 
 * For more info: https://wiki.osdev.org/Interrupt_Descriptor_Table#Structure_IA-32
 */
void isr_install()
{
    set_idt_gate(0, (uint32_t)isr0, 0x8F);
    set_idt_gate(1, (uint32_t)isr1, 0x8F);
    set_idt_gate(2, (uint32_t)isr2, 0x8F);
    set_idt_gate(3, (uint32_t)isr3, 0x8F);
    set_idt_gate(4, (uint32_t)isr4, 0x8F);
    set_idt_gate(5, (uint32_t)isr5, 0x8F);
    set_idt_gate(6, (uint32_t)isr6, 0x8F);
    set_idt_gate(7, (uint32_t)isr7, 0x8F);
    set_idt_gate(8, (uint32_t)isr8, 0x8F);
    set_idt_gate(9, (uint32_t)isr9, 0x8F);
    set_idt_gate(10, (uint32_t)isr10, 0x8F);
    set_idt_gate(11, (uint32_t)isr11, 0x8F);
    set_idt_gate(12, (uint32_t)isr12, 0x8F);
    set_idt_gate(13, (uint32_t)isr13, 0x8F);
    set_idt_gate(14, (uint32_t)isr14, 0x8F);
    set_idt_gate(15, (uint32_t)isr15, 0x8F);
    set_idt_gate(16, (uint32_t)isr16, 0x8F);
    set_idt_gate(17, (uint32_t)isr17, 0x8F);
    set_idt_gate(18, (uint32_t)isr18, 0x8F);
    set_idt_gate(19, (uint32_t)isr19, 0x8F);
    set_idt_gate(20, (uint32_t)isr20, 0x8F);
    set_idt_gate(21, (uint32_t)isr21, 0x8F);
    set_idt_gate(22, (uint32_t)isr22, 0x8F);
    set_idt_gate(23, (uint32_t)isr23, 0x8F);
    set_idt_gate(24, (uint32_t)isr24, 0x8F);
    set_idt_gate(25, (uint32_t)isr25, 0x8F);
    set_idt_gate(26, (uint32_t)isr26, 0x8F);
    set_idt_gate(27, (uint32_t)isr27, 0x8F);
    set_idt_gate(28, (uint32_t)isr28, 0x8F);
    set_idt_gate(29, (uint32_t)isr29, 0x8F);
    set_idt_gate(30, (uint32_t)isr30, 0x8F);
    set_idt_gate(31, (uint32_t)isr31, 0x8F);

    set_idt();
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t regs)
{
    char int_number[4];
    kprint("Recieved interrupt #", VGA_COLOR_WHITE_BLACK);
    // itoa();

}
