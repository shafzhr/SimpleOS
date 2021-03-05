#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../utils/utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/pci.h"
#include "../drivers/ports.h"

void kmain()
{
    isr_install();

    init_screen();
    init_keyboard();    
    clear_screen();

    kprint("Hello\n");
    pciGetDevice(0x10ec, 0x8139);
    kprint("Done\n");

}
