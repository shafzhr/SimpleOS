#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/ports.h"
#include "../utils/utils.h"
#include <stdint.h>


void PIC_remap(int offset1, int offset2);
void PIC_sendEOI(unsigned char irq);

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

    PIC_remap(0x20, 0x28);

    set_idt_gate(0x20, (uint32_t)irq0, 0x8E);
    set_idt_gate(0x21, (uint32_t)irq1, 0x8E);
    set_idt_gate(0x22, (uint32_t)irq2, 0x8E);
    set_idt_gate(0x23, (uint32_t)irq3, 0x8E);
    set_idt_gate(0x24, (uint32_t)irq4, 0x8E);
    set_idt_gate(0x25, (uint32_t)irq5, 0x8E);
    set_idt_gate(0x26, (uint32_t)irq6, 0x8E);
    set_idt_gate(0x27, (uint32_t)irq7, 0x8E);
    set_idt_gate(0x28, (uint32_t)irq8, 0x8E);
    set_idt_gate(0x29, (uint32_t)irq9, 0x8E);
    set_idt_gate(0x2A, (uint32_t)irq10, 0x8E);
    set_idt_gate(0x2B, (uint32_t)irq11, 0x8E);
    set_idt_gate(0x2C, (uint32_t)irq12, 0x8E);
    set_idt_gate(0x2D, (uint32_t)irq13, 0x8E);
    set_idt_gate(0x2E, (uint32_t)irq14, 0x8E);
    set_idt_gate(0x2F, (uint32_t)irq15, 0x8E);

    set_idt();
    asm volatile("sti");
}

static const char* exception_messages[] = {
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

static isr_handler_t handlers[256] = {0};

void register_isr_handler(int int_num, isr_handler_t handler)
{
    handlers[int_num] = handler;
}

void isr_handler(registers_t regs)
{
    char int_number_str[4];
    int int_num = regs.interrupt_n;

    if (handlers[int_num] != 0)
    {   
        isr_handler_t handler = handlers[int_num];
        handler(&regs);
        return;
    }
    
    kprint("Recieved interrupt #");

    kprint(itoa(int_num, int_number_str, 10));
    put_char('\n');
    
    kprint(exception_messages[int_num]);
    put_char('\n');
    while (1)
    {
        asm("hlt");
    }
    
}

void irq_handler(registers_t regs)
{
    int int_num = regs.interrupt_n;

    if (handlers[int_num] != 0)
    {
        // kprint("[ISR] recveived\n");
        // uint8_t buf[10];
        // kprint(itoa(int_num, buf, 16));
        // kprint("\n");
        // kprint(itoa(handlers[int_num], buf, 16));
        // kprint("\n");
        isr_handler_t handler = handlers[int_num];
        handler(&regs);
    }
    PIC_sendEOI(regs.interrupt_n - 0x20);       // subtract 0x20 - irqs offset       
}

void PIC_remap(int offset1, int offset2)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset

	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);
 
	outb(PIC1_DATA, 0x00);   // mask all
	outb(PIC2_DATA, 0x00);
}

void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}