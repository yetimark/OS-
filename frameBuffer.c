#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT		0x3D4
#define FB_DATA_PORT		0x3D5


/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND	14
#define FB_LOW_BYTE_COMMAND		15

/* Colors for fonts */
#define FB_FONT		0
#define FB_BACK		3


/* frame buffer */
char* fb = (char *) 0x000B8000;

// may need to change type, it is used for cursor and cell..
unsigned int fb_pos;


void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}

void update_cursor()
{
	fb_move_cursor(fb_pos);
}

// framebuffer for writing on screen
void fb_write_cell(unsigned int i, char c)
{
	fb[i] = c;
	fb[i + 1] = ((FB_FONT & 0x0F) << 4) | (FB_BACK & 0x0F);
}


// should work fine unless backspacing needs to be done
void fb_write(char c)
{
	fb_write_cell(fb_pos *2, c);
	fb_pos++;
	update_cursor();
}

void fb_newline()
{
	fb_pos += (80 - (fb_pos % 80));
	update_cursor();
}


void fb_backspace()
{
	fb_pos--;
	fb_write_cell(fb_pos *2, ' ');
	update_cursor();
}


void fb_clear()
{
	for(int i = 0; i < 80 *25; i++)
	{
		fb_write_cell(i*2, ' ');
	}
	fb_pos = 0;
	update_cursor();
}
