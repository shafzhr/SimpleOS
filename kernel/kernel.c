#include "../drivers/screen.h"


void kernel_main()
{
    init_screen();

    clear_screen();

    put_char('A', VGA_COLOR_WHITE_BLACK);
    put_char('\n', VGA_COLOR_WHITE_BLACK);
    kprint("Best OS!", VGA_COLOR_WHITE_BLACK);
    put_char('\n', VGA_COLOR_WHITE_BLACK);
    put_char('A', VGA_COLOR_WHITE_BLACK);

}
