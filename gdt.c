#include "gdt.h"

#define SEGMENT_COUNT 3
#define SEGMENT_BASE 0x0
#define SEGMENT_LIMIT 0xFFFFF

#define CODE_TYPE 0x9A
#define DATA_TYPE 0x92

#define FLAGS_PART 0x0C

struct gdt_entry GDT[SEGMENT_COUNT];

void gdt_make_entry(int index, unsigned int baseAddress, unsigned char access_byte)
{
    GDT[index].base_low = baseAddress & 0xFFFF;
    GDT[index].base_middle = (baseAddress >> 16) & 0xFF;
    GDT[index].base_high = (baseAddress >> 24) & 0xFF;

    GDT[index].limit_low = SEGMENT_LIMIT & 0xFFFF;
    GDT[index].limit_and_flags = (SEGMENT_LIMIT >> 16) & 0x0F;
    GDT[index].limit_and_flags |= (FLAGS_PART << 4) & 0xF0;

    GDT[index].access_byte = access_byte;
}

void init_gdt()
{
    GDT[0].base_low = 0;
    GDT[0].base_middle = 0;
    GDT[0].base_high = 0;
    GDT[0].limit_low = 0;
    GDT[0].limit_and_flags = 0;
    GDT[0].access_byte = 0;

    struct gdt_descriptor* GDT_Desc = (struct gdt_descriptor*)GDT;
    GDT_Desc->size = (sizeof(struct gdt_entry) * SEGMENT_COUNT) -1;
    GDT_Desc->offset = (unsigned int)GDT;

    gdt_make_entry(1, SEGMENT_BASE, CODE_TYPE);
    gdt_make_entry(2, SEGMENT_BASE, DATA_TYPE);

    load_gdt(*GDT_Desc);
    load_registers();
}
