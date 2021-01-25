#ifndef H_SCREEN
#define H_SCREEN

#include <stdint.h>

#define VIEDO_MEM 0xB8000

#define VGA_SCREEN_HEIGHT 25
#define VGA_SCREEN_WIDTH 80

#define CURSOR_CMD_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5

#define VGA_COLOR_WHITE_BLACK 0x0F
#define VGA_COLOR_WHITE_RED 0xF4

void init_screen(void);
void clear_screen(void);
void put_char(char, uint8_t);
void kprint_pos(char*, int, int, uint8_t);
void kprint(char*, uint8_t);


#endif