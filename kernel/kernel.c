#include "../drivers/screen.h"
#include "../utils/utils.h"

void kernel_main()
{
    char number[10]; 

    init_screen();

    clear_screen();

    put_char('A', VGA_COLOR_WHITE_BLACK);
    put_char('\n', VGA_COLOR_WHITE_BLACK);
    kprint("Best OS!", VGA_COLOR_WHITE_BLACK);
    put_char('\n', VGA_COLOR_WHITE_BLACK);
    kprint(itoa(123, number, 10), VGA_COLOR_WHITE_BLACK);
    put_char('\n', VGA_COLOR_WHITE_BLACK);

}
