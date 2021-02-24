#include "keyboard.h"
#include "screen.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "../utils/utils.h"
#include <stdint.h>

#define UNUSED(x) (void)(x)

static void keyboard_handler(registers_t* regs);
static void handle_scancode(uint8_t scancode);

static int caps_lock_state = 0;
static int shift_state = 0;

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
    int caps_lock_offset = (caps_lock_state | shift_state) * 0x20;  // 'A' == 'a' - 0x20
    switch (scancode)
    {
    case 0x2:
        put_char(shift_state ? '!':'1');
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

    case 0x10:
        put_char('q' - caps_lock_offset);
        break;
    case 0x11:
        put_char('w' - caps_lock_offset);
        break;
    case 0x12:
        put_char('e' - caps_lock_offset);
        break;
    case 0x13:
        put_char('r' - caps_lock_offset);
        break;
    case 0x14:
        put_char('t' - caps_lock_offset);
        break;
    case 0x15:
        put_char('y' - caps_lock_offset);
        break;
    case 0x16:
        put_char('u' - caps_lock_offset);
        break;
    case 0x17:
        put_char('i' - caps_lock_offset);
        break;
    case 0x18:
        put_char('o' - caps_lock_offset);
        break;
    case 0x19:
        put_char('p' - caps_lock_offset);
        break;
    case 0x1E:        
        put_char('a' - caps_lock_offset);
        break;
    case 0x1F:
        put_char('s' - caps_lock_offset);
        break;
    case 0x20:
        put_char('d' - caps_lock_offset);
        break;
    case 0x21:
        put_char('f' - caps_lock_offset);
        break;
    case 0x22:
        put_char('g' - caps_lock_offset);
        break;
    case 0x23:
        put_char('h' - caps_lock_offset);
        break;
    case 0x24:
        put_char('j' - caps_lock_offset);
        break;
    case 0x25:
        put_char('k' - caps_lock_offset);
        break;
    case 0x26:
        put_char('l' - caps_lock_offset);
        break;
    case 0x2C:
        put_char('z' - caps_lock_offset);
        break;
    case 0x2D:
        put_char('x' - caps_lock_offset);
        break;
    case 0x2E:
        put_char('c' - caps_lock_offset);
        break;
    case 0x2F:
        put_char('v' - caps_lock_offset);
        break;
    case 0x30:
        put_char('b' - caps_lock_offset);
        break;
    case 0x31:
        put_char('n' - caps_lock_offset);
        break;
    case 0x32:
        put_char('m' - caps_lock_offset);
        break;

    case 0xE:
        kprint_backspace();
        break;
    case 0xF:
        kprint("    ");
        break;
    case 0x1C:
        kprint("\n");
        break;
    case 0x39:
        kprint(" ");
        break;    

    case 0x3A:                  /* caps lock pressed */
        caps_lock_state ^= 1;   /* toggle caps-lock state */
        break;
    case 0x2A:                  /* left shift pressed */
    case 0x36:                  /* right shift pressed */
        shift_state |= 1;        
        break;
    case 0xAA:                  /* left shift released */
    case 0xB6:                  /* right shift released */
        shift_state &= 0;        
        break;
    
    default:
        break;
    }
}

