/**
 * @brief screen output 
 * https://wiki.osdev.org/Text_Mode_Cursor
 */
#include "screen.h"
#include "ports.h"
#include "../utils/string.h"
#include <stdint.h>
#include <stdbool.h>


bool validate_pos(int, int);
uint16_t calc_offset(int, int);
uint16_t get_cursor_pos(void);
uint16_t calc_ch_attr(char, uint8_t);
void set_cursor_pos(uint16_t);
int calc_row(uint16_t);
void put_char_pos(char, int, int, uint8_t);


/**************************************
 * Public                             *
 *************************************/

/**
 * @brief Initialize cursor position to 0
 */
void init_screen(void)
{
    set_cursor_pos(0);
}

/**
 * @brief Prints a massage at the cursor's position
 * 
 * @param msg massage to print
 * @param attribute VGA memory attribute
 */
void kprint(char* msg, uint8_t attribute)
{
    kprint_pos(msg, -1, -1, attribute);
}

void put_char(char ch, uint8_t attribute)
{
    put_char_pos(ch, -1, -1, attribute);
}

void clear_screen(void)
{
    uint16_t* video_mem = (uint16_t*)VIEDO_MEM;
    int screen_size = VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH;
    for (int i = 0; i < screen_size; i++)
    {
        video_mem[i] = calc_ch_attr(' ', VGA_COLOR_WHITE_BLACK);
    }
    set_cursor_pos(0);
}

/**
 * @brief Prints a char array to a given position.
 *        If the given position is negative the position
 *        would be the cursor's position.
 *        For more info: https://en.wikipedia.org/wiki/VGA_text_mode
 * 
 * @param msg a pointer to a sequence of chars that ends with 0
 * @param x starting column
 * @param y starting row
 * @param attribute attribute of the video memory bytes
 */
void kprint_pos(char* msg, int x, int y, uint8_t attribute)
{
    if (validate_pos(x, y))
    {
        set_cursor_pos(calc_offset(x, y));
    }
    
    for (; *msg != 0; ++msg)
    {
        put_char(*msg, attribute);
    }
    
}

/**************************************
 * Private                            *
 *************************************/

/**
 * @brief puts a char in the screen in a given position.
 *        If the given position is negative the position
 *        would be the cursor's position.
 *        For more info: https://en.wikipedia.org/wiki/VGA_text_mode
 * 
 * @param ch character to print
 * @param x column
 * @param y row
 * @param attribute character's attribute
 */
void put_char_pos(char ch, int x, int y, uint8_t attribute)
{
    uint16_t* video_mem = (uint16_t*)VIEDO_MEM;
    if (x >= VGA_SCREEN_WIDTH || y >= VGA_SCREEN_HEIGHT)
    {
        kprint("E: Position out of range", VGA_COLOR_WHITE_RED); // TODO: Add the wrong range itself after "itoa" is implemented
        return;
    }
    uint16_t offset = (validate_pos(x, y)) ? calc_offset(x, y) : get_cursor_pos();

    switch (ch)
    {
        int row;
    case '\n':
        row = calc_row(offset);
        offset = calc_offset(0, row+1);
        break;
    case 0x8:
        video_mem[offset] = calc_ch_attr(' ', attribute);
        break;
    default:
        video_mem[offset] = calc_ch_attr(ch, attribute);
        ++offset;
        break;
    }
    if (offset >= VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH)
    {
        memcpy(video_mem, video_mem + VGA_SCREEN_WIDTH, (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH * 2);

        uint16_t* last_line = video_mem + (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH;
        for (size_t i = 0; i < VGA_SCREEN_WIDTH; i++)
        {
            last_line[i] = calc_ch_attr(' ', VGA_COLOR_WHITE_BLACK);   
        }
        
        offset = (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH;
    }

    set_cursor_pos(offset);
}


/**
 * @brief get current screen's cursor position
 * 
 * @return cursor's position
 */
uint16_t get_cursor_pos(void)
{
    uint16_t pos = 0;
    port_out(CURSOR_CMD_PORT, 0xF);
    pos |= port_in(CURSOR_DATA_PORT);
    port_out(CURSOR_CMD_PORT, 0xE);
    pos |= ((uint16_t)port_in(CURSOR_DATA_PORT)) << 8;
    return pos;
}

/**
 * @brief set current screen's cursor position
 * 
 * @param offset offset
 */
void set_cursor_pos(uint16_t offset)
{
    port_out(CURSOR_CMD_PORT, 0xF);
    port_out(CURSOR_DATA_PORT, (uint8_t) (offset & 0xFF));
    port_out(CURSOR_CMD_PORT, 0xE);
    port_out(CURSOR_DATA_PORT, (uint8_t) ((offset >> 8) & 0xFF));
}

uint16_t calc_offset(int x, int y)
{
    return y * VGA_SCREEN_WIDTH + x;
}

int calc_row(uint16_t offset)
{
    return offset / VGA_SCREEN_WIDTH;
}

bool validate_pos(int x, int y)
{
    return (
            x >= 0 && y >= 0
            && x < VGA_SCREEN_WIDTH && y < VGA_SCREEN_HEIGHT
        );
}

uint16_t calc_ch_attr(char ch, uint8_t attr)
{
    return ((uint16_t)attr << 8) | ch;
}