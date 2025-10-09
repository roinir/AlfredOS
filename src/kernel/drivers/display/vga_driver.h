#ifndef VGA_DRIVER
#define VGA_DRIVER

#include <stdint.h>

void set_cursor(int offset);

unsigned int get_cursor(void);

void print_char(unsigned char character, const char* color, unsigned int offset);

void print_char_default_color(unsigned char character, unsigned int offset);

uint8_t _string_to_vga_color(const char* color);

void print_string(char* msg, const char* color);

unsigned int get_offset_to_new_line(unsigned int offset);

void scroll_down();

void copy_chunck(volatile char* src, volatile char* dst, int len);

void fill_row_with_char(unsigned char character);

void fill_screen_with_char(unsigned char character);

void clear_screen();

enum vga_color {
        BLACK = 0x0,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHT_GRAY,
        DARK_GRAY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_MAGENTA,
        YELLOW,
	WHITE
};

#endif
