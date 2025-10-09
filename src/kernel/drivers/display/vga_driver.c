#include <string.h>

#include "../IO/io_driver.h"
#include "vga_driver.h"
#include "../../../utils/string.h"

#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5
#define VGA_OFFSET_HIGH 14
#define VGA_OFFSET_LOW 15
#define VIDEO_ADDRESS 0xB8000
#define NUM_OF_ROWS 25
#define NUM_OF_COLS 80
#define NULL_CHAR ' '


/**
 * @brief Move the VGA hardware test-mode cursor to a given position.
 *
 * The VGA text mode cursor position is controlled by the CRT Controller
 * (CRTC) registers 0x0E (cursor high byte) and 0x0F (cursor low byte).
 * To set the cursor position, we:
 * 	1. Convert the byte offset into a character-cell offset (offset/2).
 *      2. Write the high 8 bits of the cell offset to register 0x0E.
 *      3. Write the low  8 bits of the cell offset to register 0x0F.
 * 
 * Example:
 *   // Move cursor to the start of the second line (row 1, col 0)
 *   int byte_offset = 2 * (1 * 80 + 0); // row*cols + col, times 2 bytes
 *   set_cursor(byte_offset);
 *
 * @offset: The byte offset into video memory (starting at 0xB8000) where
 * the cursor should be placed. Must be an even number since each
 * text cell uses 2 bytes (character + attribute).
 * 
 * @return No return val
 */
void set_cursor(int offset) {
	offset /= 2;

	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset >> 8));

	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	port_byte_out(VGA_DATA_REGISTER, (unsigned char)(offset & 0xFF));
}


/**
 * get_cursor - Read the current position of the VGA hardware text-mode cursor.
 *
 * The VGA text mode cursor position is stored in the CRT Controller
 * (CRTC) registers 0x0E (cursor high byte) and 0x0F (cursor low byte).
 * To read the cursor position, we:
 *      1. Read the high 8 bits of the cursor offset from register 0x0E.
 *      2. Read the low  8 bits of the cursor offset from register 0x0F.
 *      3. Combine them into a 16-bit character-cell offset.
 *      4. Multiply by 2 to convert from cell offset to byte offset in video memory.
 *
 * Example:
 *   int cursor_byte_offset = get_cursor();
 *   // cursor_byte_offset can now be used to write characters at the current cursor location
 *
 * @return The byte offset into video memory (starting at 0xB8000)
 *         corresponding to the current cursor position.
 */
unsigned int get_cursor(void) {
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	unsigned char high_pos = port_byte_in(VGA_DATA_REGISTER);
	
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	unsigned char low_pos = port_byte_in(VGA_DATA_REGISTER);
	
	unsigned int pos = ((unsigned int)high_pos << 8) | (unsigned int)low_pos;
	return pos * 2;
}


/**
 * @brief Print a character at a specific offset in VGA text mode
 * 
 * @character: The ASCII character to display
 * @color: A string representing the foreground color (e.g., "red", "blue")
 * @offset: The byte offset into video memory (0xB8000)
 *
 * Notes:
 *   - Each text cell is 2 bytes: character + attribute
 *   - The attribute byte encodes foreground (low 4 bits) and background (high 4 bits)
 * 
 * @return No return val
 */
void print_char(unsigned char character, const char* color, unsigned int offset) {
    volatile char* video_base = (volatile char*)VIDEO_ADDRESS; // VGA text memory

    uint8_t color_byte = _string_to_vga_color(color);

    video_base[offset] = character;          // character byte
    video_base[offset + 1] = color_byte;     // attribute byte
}

/**
 * @brief Print a character at a specific offset in VGA text mode with the default color
 * 
 * @character: The ASCII character to display
 * @color: A string representing the foreground color (e.g., "red", "blue")
 * @offset: The byte offset into video memory (0xB8000)
 *
 * Notes:
 *   - Each text cell is 2 bytes: character + attribute
 *   - The attribute byte encodes foreground (low 4 bits) and background (high 4 bits)
 * 
 * @return No return val
 */
void print_char_default_color(unsigned char character, unsigned int offset) {
    volatile char* video_base = (volatile char*)VIDEO_ADDRESS; // VGA text memory

    uint8_t color_byte = WHITE; // default color

    video_base[offset] = character;          // character byte
    video_base[offset + 1] = color_byte;     // attribute byte
}

/**
 * @brief Convert a color name string to a VGA color code
 * @color: A string representing the VGA foreground color. Valid values are:
 * 	"black", "blue", "green", "cyan", "red", "magenta", "brown",
 * 	"light_gray", "dark_gray", "light_blue", "light_green",
 * 	"light_cyan", "light_red", "light_magenta", "yellow", "white".
 *
 * This function maps a human-readable color name string to its
 * corresponding VGA color code (0x0–0xF). Each VGA text-mode character
 * cell uses one attribute byte to encode foreground and background
 * colors. This function returns only the 4-bit foreground color code.
 *
 * @return The 4-bit VGA color code (0–15) corresponding to the given color name.
 * If the string does not match any known color, LIGHT_GRAY (0x7) is returned
 * as a default.
 *
 * Example:
 *   uint8_t color_code = _string_to_vga_color("red"); // returns 0x4
 *
 * Notes:
 *   - Only the foreground color is returned. Background colors must be handled separately.
 */
uint8_t _string_to_vga_color(const char* color) {
    	if (strcasecmp(color, "black") == 0)       return BLACK;
    	else if (strcasecmp(color, "blue") == 0)   return BLUE;
    	else if (strcasecmp(color, "green") == 0)  return GREEN;
    	else if (strcasecmp(color, "cyan") == 0)   return CYAN;
    	else if (strcasecmp(color, "red") == 0)    return RED;
    	else if (strcasecmp(color, "magenta") == 0) return MAGENTA;
    	else if (strcasecmp(color, "brown") == 0)  return BROWN;
   	else if (strcasecmp(color, "light_gray") == 0) return LIGHT_GRAY;
    	else if (strcasecmp(color, "dark_gray") == 0)  return DARK_GRAY;
    	else if (strcasecmp(color, "light_blue") == 0) return LIGHT_BLUE;
    	else if (strcasecmp(color, "light_green") == 0) return LIGHT_GREEN;
    	else if (strcasecmp(color, "light_cyan") == 0) return LIGHT_CYAN;
    	else if (strcasecmp(color, "light_red") == 0) return LIGHT_RED;
    	else if (strcasecmp(color, "light_magenta") == 0) return LIGHT_MAGENTA;
    	else if (strcasecmp(color, "yellow") == 0) return YELLOW;
    	else if (strcasecmp(color, "white") == 0)  return WHITE;
	return WHITE;
}

/**
 * @brief Print a null-terminated string to the VGA text buffer
 * @msg:   Pointer to the null-terminated string to print.
 * @color: VGA color code (0x0–0xF) or mapped color string to use for the text.
 *
 * This function writes the given string to VGA text-mode memory starting
 * at the current cursor position. Each character is written as a pair of
 * bytes: one for the ASCII character and one for the color attribute.
 * The cursor is advanced after each character and updated when the string
 * has finished printing.
 *
 * @return No return value.
 *
 * Example:
 *   print_string("Hello, world!", "light_green");
 *
 * Notes:
 *   - Each VGA text cell uses 2 bytes (character + attribute).
 *   - Cursor wrapping and screen scrolling behavior are not handled here
 *     and must be implemented separately if needed.
 *   - Assumes standard 80x25 VGA text mode at memory address 0xB8000.
 */
void print_string(char* msg, const char* color){
	unsigned int offset = get_cursor();
	int extra_rows = 0;
	for (int i = 0; i < strlen(msg); i++) {
		offset += 2;
		if (msg[i] == '\n'){
			offset += get_offset_to_new_line(offset);
		}
		if (offset >= NUM_OF_ROWS * (NUM_OF_COLS * 2 + extra_rows)){
			scroll_down();
			offset -= NUM_OF_ROWS;
		}
		print_char(msg[i], color, offset);
	}
	set_cursor(offset);
}

unsigned int  get_offset_to_new_line(unsigned int offset){
	return NUM_OF_ROWS * 2 - (offset % (2 * NUM_OF_ROWS));
}

void scroll_down(){
	unsigned int last_row = 2 * NUM_OF_ROWS * (NUM_OF_COLS - 1);
	volatile char* video_base = (volatile char*)VIDEO_ADDRESS; // VGA text memory	
	copy_chunck(&(*(video_base + NUM_OF_ROWS * 2)), video_base, 2 * (NUM_OF_ROWS * (NUM_OF_COLS - 1)));
	set_cursor(last_row);
	fill_row_with_char(NULL_CHAR);
}

void copy_chunck(volatile char* src, volatile char* dst, int len){
	for (int i = 0; i < len; i++){
		*(src + i) = *(dst + i);
	}
}

void fill_row_with_char(unsigned char character){
	for (int i = 0; i < NUM_OF_ROWS; i += 2){
		print_char_default_color(character, i);
	}
}

void fill_screen_with_char(unsigned char character){
	set_cursor(0);
	for (int i = 0; i < NUM_OF_COLS; i++){
		fill_row_with_char(character);
		set_cursor(i * NUM_OF_ROWS * 2);
	}
}

void clear_screen(){
	fill_screen_with_char(NULL_CHAR);
}
