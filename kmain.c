#include "serialPorts.h"
#include "Mem_Manager.h"
#include "frameBuffer.h"
#include "gdt.h"
#include "inter.h"







int main(void)
{
	//prepare os for handing out memory
	//unmarkAllMemory();
	init_gdt();
	init_idt();
	fb_clear();
	//fb_write_cell(0, 'B');
}
