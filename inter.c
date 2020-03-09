#include "io.h"
#include "frameBuffer.h"
#include "inter.h"
#include "pic.h"

#define KB_DATA_PORT 0x60

struct idt_ptr idtp;
struct idt_entry idt[256];

static unsigned char scan_code_to_ascii[256] =
{
    0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x0,  // 0-14
    0x0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',      // 15-28
    0x0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0x0, '\\',// 29-43
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0x0, '*', 0x0, ' ',       // 44-57
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,            // 58-70
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'             // 71-83
};


void init_entry(int index, unsigned int address)
{
    idt[index].offset_low = address & 0xFFFF;
    idt[index].offset_high = (address >> 16) & 0xFFFF;
    idt[index].segment_selector = 0x08;
    idt[index].reserved = 0x00;

    //                (     P   )   (       DPL             )
    idt[index].pdpl = (0x01 << 7) | (0x00 << 6) | (0x00 << 5) | 0xE;
}

void init_idt()
{
    init_entry(33, (unsigned int)interrupt_handler_33);

    idtp.size = sizeof(struct idt_entry) * 256;
    idtp.address = (int)&idt;

    load_idt((int) &idtp);

    pic_remap(0x20, 0x28);
}


void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, unsigned int interrupt, __attribute__((unused)) struct stack_state stack)
{
    if(interrupt == 33)
    {
        unsigned char scan_code = inb(KB_DATA_PORT);
        char ascii = scan_code_to_ascii[scan_code];

        if(ascii != 0x0)
        {
            if(ascii == '\n') {
                fb_newline();
            }
            else if (scan_code == 72){
                arrow_up();
            }
            else if (scan_code == 75){
                arrow_left();
            }
            else if (scan_code == 77){
                arrow_right();
            }
            else if (scan_code == 80){
                arrow_down();
            }
            else {
                fb_write(ascii);
            }
            pic_acknowledge(interrupt);
        }
        else
        {
            if(scan_code == 14){
                fb_backspace();
            }
            pic_acknowledge(interrupt);
            return;
        }
    }

}
