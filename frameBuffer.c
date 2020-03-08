#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT		0x3D4
#define FB_DATA_PORT		0x3D5


/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND	14
#define FB_LOW_BYTE_COMMAND	15

/* Colors for fonts */
#define FB_FONT		0
#define FB_BACK		3


/* frame buffer */
char* fb = (char *) 0x000B8000;


void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}


// framebuffer for writing on screen
void fb_write_cell(unsigned int i, char c)
{
	fb[i] = c;
	fb[i + 1] = ((FB_FONT & 0x0F) << 4) | (FB_BACK & 0x0F);
}


void fb_write_string(int offset, char* s, int length)
{
	for(int i = 0; i < length; i++)
	{
		fb_write_cell(offset + i*2, s[i]);
	}
}


void fb_clear()
{
	for(int i = 0; i < 80 *25; i++)
	{
		fb_write_cell(i*2, ' ');
	}
}
