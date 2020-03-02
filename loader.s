global loader

KERNEL_STACK_SIZE	equ 4096			; align at 4 bytes
MAGIC_NUMBER		equ 0x1BADB002			
FLAGS			equ 0x0
CHECKSUM		equ -MAGIC_NUMBER

section .bss
align 4
kernel_stack:
	resb KERNEL_STACK_SIZE

extern main

section .text						; start of the (code) section
align 4							
	dd MAGIC_NUMBER					
	dd FLAGS
	dd CHECKSUM

loader:
	mov esp, kernel_stack + KERNEL_STACK_SIZE
	call main
.loop:
	jmp .loop
