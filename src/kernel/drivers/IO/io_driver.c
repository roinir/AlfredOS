/**
 * @brief Reads a byte from the specified port.
 *
 * This function preforms a I/O read operation using the 'in'
 * assembly instruction. It is used to retrieve a byte of data from a
 * hardware device register, such as VGA controller or keyboard controller.
 *
 * @parm port The 16-bit I/O port address to read from.
 * @return The byte value read from the specified I/O port.
 */
unsigned char port_byte_in(unsigned short port){
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}


/**
 * @brief Writes a byte into the specified port.
 *
 * This function preforms a I/O write operation using the 'out'
 * assembly instruction. It is used to write a byte of data to be read by a
 * hardware device register, such as VGA controller or keyboard controller.
 *
 * @parm port The 16-bit I/O port address to write to.
 * @parm data The data to write to the port.
 * @return no return val.
 */
void port_byte_out(unsigned short port, unsigned char data){
	__asm__("out %%al, %%dx" : : "a" (data), "d" (data));
}
