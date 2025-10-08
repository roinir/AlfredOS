void kmain(void) {
	int j;
	for (int i = 0; i < 100; i++){
		j = i;	
	}
	char *vga = (char*)0xb8000;   // VGA text buffer
	vga[0] = 'K';                 // char
	vga[1] = 0x07;                // attribute: white on black
}
