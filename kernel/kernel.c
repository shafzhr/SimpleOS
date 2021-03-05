#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../utils/utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/pci.h"
#include "../drivers/ports.h"
#include "../drivers/rtl8139.h"

void kmain()
{
    isr_install();

    init_screen();
    init_keyboard();    
    clear_screen();

    kprint("Hello\n");

    rtl8139_init();

    
    kprint("Done\n");
}
