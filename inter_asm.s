extern interrupt_handler

global interrupt_handler_33

interrupt_handler_33:
    push    dword 0
    push    dword 33
    jmp common_interrupt_handler

common_interrupt_handler:
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    ebp
    push    esi
    push    edi

    call    interrupt_handler

    pop     edi
    pop     esi
    pop     ebp
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

    add     esp, 8

    iret



global load_idt

load_idt:
    mov eax, [esp + 4]
    lidt[eax]
    ret
