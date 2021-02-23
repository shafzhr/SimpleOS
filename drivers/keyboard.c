#include "keyboard.h"
#include "screen.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "../utils/utils.h"
#include <stdint.h>

#define UNUSED(x) (void)(x)

static void keyboard_handler(registers_t* regs);
static void handle_scancode(uint8_t scancode);


void init_keyboard(void)
{
    register_isr_handler(IRQ1, keyboard_handler);
}

static void keyboard_handler(registers_t* regs)
{
    UNUSED(regs);
    uint8_t scancode = inb(0x60);
    handle_scancode(scancode);
    
}

static void handle_scancode(uint8_t scancode)
{
    switch (scancode)
    {
    case 0x2:
        kprint("1");
        break;
    case 0x3:
        kprint("2");
        break;
    case 0x4:
        kprint("3");
        break;
    case 0x5:
        kprint("4");
        break;
    case 0x6:
        kprint("5");
        break;
    case 0x7:
        kprint("6");
        break;
    case 0x8:
        kprint("7");
        break;
    case 0x9:
        kprint("8");
        break;
    case 0xA:
        kprint("9");
        break;
    case 0xB:
        kprint("0");
        break;

    case 0xE:
        kprint_backspace();
        break;
    case 0xF:
        kprint("    ");
        break;

    case 0x10:
        kprint("Q");
        break;
    case 0x11:
        kprint("W");
        break;
    case 0x12:
        kprint("E");
        break;
    case 0x13:
        kprint("R");
        break;
    case 0x14:
        kprint("T");
        break;
    case 0x15:
        kprint("Y");
        break;
    case 0x16:
        kprint("U");
        break;
    case 0x17:
        kprint("I");
        break;
    case 0x18:
        kprint("O");
        break;
    case 0x19:
        kprint("P");
        break;
    case 0x1C:
        kprint("\n");
        break;
    case 0x1E:
        kprint("A");
        break;
    case 0x1F:
        kprint("S");
        break;
    case 0x20:
        kprint("D");
        break;
    case 0x21:
        kprint("F");
        break;
    case 0x22:
        kprint("G");
        break;
    case 0x23:
        kprint("H");
        break;
    case 0x24:
        kprint("J");
        break;
    case 0x25:
        kprint("K");
        break;
    case 0x26:
        kprint("L");
        break;
    case 0x2C:
        kprint("Z");
        break;
    case 0x2D:
        kprint("X");
        break;
    case 0x2E:
        kprint("C");
        break;
    case 0x2F:
        kprint("V");
        break;
    case 0x30:
        kprint("B");
        break;
    case 0x31:
        kprint("N");
        break;
    case 0x32:
        kprint("M");
        break;

    case 0x39:
        kprint(" ");
        break;    
    
    default:
        break;
    }
}

