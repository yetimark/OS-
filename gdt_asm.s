
global load_gdt
global load_registers

load_gdt:
    lgdt [esp + 4]
    ret

load_registers:
    mov ax, 0x10
    mov ds, ax      ;(unk. ctxt)   8ed8
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    jmp 0x08:flush_cs

flush_cs:
    ret
