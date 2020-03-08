#include "serialPorts.h"
#include "Mem_Manager.h"
#include "frameBuffer.h"







int main(void)
{
	//prepare os for handing out memory
	unmarkAllMemory();

	fb_clear();
	fb_write_cell(0, 'B');
	fb_move_cursor(0);
}
