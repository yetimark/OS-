#include "serialPorts.h"
#include "Mem_Manager.h"
#include "frameBuffer.h"
#include "gdt.h"







int main(void)
{
	//prepare os for handing out memory
	unmarkAllMemory();
	gdt_init(); 		//crashes caused by gdt, just use mem unsegmented?

	fb_clear();
	fb_write_cell(0, 'B');
	fb_move_cursor(0);
}
