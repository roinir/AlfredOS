#ifndef IO_DRIVER
#define IO_DRIVER

/**
 * @brief Reads a byte from the specified port.
 * @parm port The 16-bit I/O port address to read from.
 * @return The byte value read from the specified I/O port.
 */
unsigned char port_byte_in(unsigned short port);


/**
 * @brief Writes a byte into the specified port.
 * @parm port The 16-bit I/O port address to write to.
 * @parm data The data to write to the port.
 * @return no return val.
 */
void port_byte_out(unsigned short port, unsigned char data);

#endif
