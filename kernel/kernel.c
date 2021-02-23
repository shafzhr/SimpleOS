#include "../drivers/screen.h"
#include "../utils/utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"


void kmain()
{
    isr_install();

    init_screen();
    clear_screen();

    int x = 5 / 0;
}
