#include "../drivers/screen.h"
#include "../utils/utils.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void kernel_main()
{
    isr_install();

    init_screen();
    clear_screen();

    asm volatile("int 2");
    asm volatile("int 3");
}
