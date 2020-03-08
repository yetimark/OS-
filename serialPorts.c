#include "io.h"

#define SERIAL_COM1_BASE			0x3F8	/* COM1 base port */

#define SERIAL_DATA_PORT(base)			(base)
#define SERIAL_FIFO_COMMAND_PORT(base)		(base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)		(base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)		(base + 4)
#define SERIAL_LINE_STATUS_PORT(base)		(base + 5)

#define SERIAL_LINE_ENABLE_DLAB			0x80


void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com),
			SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com),
			(divisor >> 8) & 0x00FF);
	outb(SERIAL_DATA_PORT(com),
			(divisor & 0x00FF));
}

void serial_configure_line(unsigned short com)
{
	/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
         * Content: | d | b | prty  | s | dl  |
         * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
	 */
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}




int serial_is_transmit_fifo_empty(unsigned int com)
{
	/* 0x02 = 0010 0000 */
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x02;
	//return 0;
}
