#include "../drivers/display/vga_driver.h"

void kmain(void) {
	clear_screen();
	print_string("Hello world!\n", "white");
}
