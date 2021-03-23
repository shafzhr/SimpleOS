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
    init_screen();
    clear_screen();
    isr_install();

    init_keyboard();    

    kprint("Hello\n");

    rtl8139_init();
    // rtl8139_send_packet("\xFF\xFF\xFF\xFF\xFF\xFF\x52\x55\x00\xd1\x55\x01\x08\x00", 14);
    rtl8139_send_packet("\xff\xff\xff\xff\xff\xff\x52\x55\x00\xd1\x55\x01\x08\x06\x00\x01\x08\x00\x06\x04\x00\x01\x52\x55\x00\xd1\x55\x01\x0a\x00\x02\x0f\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x0a", 42);
    
    kprint("Done\n");
}
