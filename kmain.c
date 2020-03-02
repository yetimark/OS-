#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT		0x3D4
#define FB_DATA_PORT		0x3D5


/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND	14
#define FB_LOW_BYTE_COMMAND	15

#define FB_FONT		5
#define FB_BACK		3

void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}


/* frame buffer */
char* fb = (char *) 0x000B8000;

/* memory spaces */
char* dataMarkStart = (char*) 0x00200000;
char* dataMarkEnd = (char*) 0x010FF7FE;
char* dataRealStart = (char*) 0x010FF7FF;
char* dataRealEnd = (char*)


// framebuffer for writing on screen
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	fb[i] = c;
	fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}


// interface between OS and framebuffer
/*
int write(char *buf, unsigned int len)
{
	// writes the contents of the buffer 'buf' of length 'len' to the screen.
	// should automatically advance the cursor after a character has been written
	// and scroll the screen if necessary.. make it.

	return 0;
}
*/

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

void fb_write_string(int offset, char* s, int length)
{
	for(int i = 0; i < length; i++)
	{
		fb_write_cell(offset + i*2, s[i], FB_FRONT, FB_BACK);
	}
}

void fb_clear()
{
	for(int i = 0; i < 80 *25; i++)
	{
		fb_write_cell(i*2, ' ', FB_FRONT, FB_BACK);
	}
}

int isMemAvailable(char* baseAddress, int size)
{
	for(char* c = baseAddress; c < (baseAddress + size); c++)
	{
		if (*c == 1)
		{
			return 0; //this base address won't work out
		}
	}
	return 1; //this base address will
}

void markMemory(char* baseAddress, int size)
{
	for(char* c = baseAddress; c < baseAddress + size; c++)
	{
		*c = 1;
	}
}

// double check litman's git
char* getMem(unsigned int numBytes)
{
	int dataMapOffset = (int)dataRealStart - (int)dataMarkStart;
	for(char* c = dataMarkStart; c <= dataMarkEnd; c++)
	{
		if(*c == 0)
		{
			if (isMemoryAvailable(c, numBytes))
			{
				markMemory(c, numBytes);
				return c + dataMapOffset;
			}
		}
	}
}

void freeMem(char* baseAddress, int numBytes)
{
	char* c = baseAddress;
	for(int i = 0; i < numBytes; i++)
	{
		*(baseAddress)
	}
}

void unmarkAllMemory()
{
	for(char* c = dataMarkStart; c <= dataMarkEnd; c++)
	{
		*c = 0;
	}
}

int main(void)
{
	//prepare os for handing out memory
	unmarkAllMemory();

	fb_clear();
	fb_write_cell(0, 'B', 5, 3);
	fb_move_cursor(0);

	char* s = bucketStart;
	s[0] = 'A';
	s[1] = 'm';
	s[2] = 'e';
	s[3] = 't';
	s[4] = 'u';
	s[5] = 'e';
	s[6] = 'r';
	s[7] = ' ';
	s[8] = 'R';
	s[9] = 'o';
	*(s + 10) = 'c';
	*(s + 11) = 'k';
	*(s + 12) = 'e';
	s[13] = 't';
	s[14] = 'r';
	s[15] = 'y';

	for (int i = 0; i < 16; i++)
	{
		fb_write_cell(i*2, s[i], 5, 3);
	}

	for(int i = 0; i < 25; i++)
	{
		s = getMem(5);
		//strcpy(s, "hello", )
	}
}
