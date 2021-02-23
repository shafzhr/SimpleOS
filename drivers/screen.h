#ifndef H_SCREEN
#define H_SCREEN

#include <stdint.h>

#define VIEDO_MEM 0xB8000

#define VGA_SCREEN_HEIGHT 25
#define VGA_SCREEN_WIDTH 80

#define CURSOR_CMD_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5

#define VGA_COLOR_WHITE_BLACK 0x0F
#define VGA_COLOR_WHITE_RED 0x4F

void init_screen(void);
void clear_screen(void);
void put_char(const char, uint8_t);
void kprint_pos(const char*, int, int, uint8_t);
void kprint(const char*);
void kprint_backspace(void);

#endif