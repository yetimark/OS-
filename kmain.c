#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT		0x3D4
#define FB_DATA_PORT		0x3D5


/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND	14
#define FB_LOW_BYTE_COMMAND	15

void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}


/* frame buffer */
char* fb = (char *) 0x000B8000;


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

void fb_write_string()
{
	//clip at 1:13
}

void fb_clear()
{
	for(int i = 0; i < 80 *25; i++)
	{
		fb_write_cell(i*2, ' ', 5, 3);
	}
}

/* start of memory space to store stuff */
char* bucketStart = (char *) 0x00300000;

/* Start of memory space for the mid level get mem function */
char* byteFlagStart = (char *) 0x00E7F800;

/* start of memory availability flags (specifically the packed version) or the hard getmem*/
char* flagStart = (char *) 0x01C5F200; 
// stack space begins at 0x01FFF000 I believe


/* do not worry about running out of memory 
	returns next memory address and advances the pointer forwards
	HAS NO CHECKING */
char* easyGetMem(unsigned int numberOfBuckets)
{
	char* returnAddress = bucketStart;
	/*advance bucket pointer ony byte for each bucket requested */
	while (numberOfBuckets > 0)
	{
		bucketStart += 0x00000008;
		numberOfBuckets--;
	}
	//bucketStart += 0x00000008;

	return returnAddress;
}

/* splits muckets, halfemory in half. half for b for bucket flags 
	also checks to make sure space is available*/
char* meduimGetMem(unsigned int numberOfBuckets)
{
	char* returnAddress = bucketStart;
	char* currFlag = byteFlagStart;

	unsigned int count = 0;
	unsigned int tracker = 0;

	/* Starting from bucketStart. Check for available memory. Once it is found
		make sure it has enough space behind it and flag the buckets
		return the starting address for the space*/

	while (count < numberOfBuckets)
	{
		// 0xFFFFFFFF is the value I am using for "unavailable"
		if (&currFlag != 0xFFFFFFFF)
		{
			count++;
		}
		else 
		{
			// tracker should only be added to at the last possible starting spot
			tracker += count;

			// count is reset once a flag is hit
			count = 0;
		}

		// move the pointer to the next spot always
		currFlag += 0x00000008;
	}

	// not sure how C works with decimal and hex multiplying but python does fine
	returnAddress += (0x00000008 * tracker);
	return returnAddress;
}


char* getMem(unsigned int numberOfBuckets) 
{
	// Return the base address where that much memory can be used
	// Mark that the memory there is in use

	// separate memory into 7/8 and 1/8 
	// memory is given out in byte sized buckets
	// memory is declared "used" by a single bit in a reserved section
	// if buckets 1 and 3 are "used" then reserved: 00000101 -> 5

	// only placed here to make the compiler happy for temp
	//char* returnAddress = easyGetMem(numberOfBuckets);
	char* returnAddress = mediumGetMem(numberOfBuckets);

	return returnAddress;
}

int main(void)
{
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
}
