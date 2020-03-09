struct cpu_state {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
}__attribute__((packed));

struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
}__attribute__((packed));

struct idt_ptr {
    unsigned short size;
    unsigned int address;
}__attribute__((packed));

struct idt_entry {
    unsigned short offset_low;
    unsigned short segment_selector;
    unsigned char reserved;
    unsigned char pdpl;
    unsigned short offset_high;
}__attribute__((packed));



void init_idt();

void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);

void interrupt_handler_33();
void load_idt(unsigned int idt_address);
