struct gdt_descriptor {
    unsigned short size;
    unsigned int offset;
}__attribute__((packed));

struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access_byte;
    unsigned char limit_and_flags;
    unsigned char base_high;
}__attribute__((packed));


void init_gdt();
void load_gdt(struct gdt_descriptor GDT);
void load_registers();
