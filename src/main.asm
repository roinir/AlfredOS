org 0x7c00
bits 16

%define ENDL 0x0D, 0x0A

CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDR equ 0x100000

start:
	jmp main

puts:
	push si
	push ax
.loop:
	lodsb
	or al, al
	jz .done

	mov ah, 0x0d
	mov bh, 0
	int 0x10

	jmp .loop

.done:
	pop ax
	pop si
	ret

main:
	; can't write to ds/es directly
	mov ax, 0
	mov ds, ax
	mov es, ax

	; setup stack
	mov ss, ax
	mov sp, 0x7C00

	mov si, msg_hello
	call puts
	
	;load kernel
	mov bx, KERNEL_LOAD_SEG

	mov dh, 0x00

	mov dl, 0x80

	mov cl, 0x02

	mov ch, 0x00

	mov ah, 0x02

	mov al, 8

	int 0x13

	jc disk_read_error


load_PM:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or al, 1
	mov cr0, eax
	jmp CODE_OFFSET:PModeMain

disk_read_error:
	mov si, msg_disk_error
	call puts	
	hlt
	jmp $

gdt_start:
	dd 0x0
	dd 0x0

	;code segment descriptor
	dw 0xFFFF    ;limit
	dw 0x0000    ;base
	dw 0x00      ;base
	db 10011010b ;access byte
	db 11001111b ;flags
	db 0x00      ;base	
	
	;data segment descriptor
	dw 0xFFFF    ;limit
	dw 0x0000    ;base
	dw 0x00      ;base
	db 10010010b ;access byte
	db 11001111b ;flags
	db 0x00      ;base

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ;size of gdt minus 1
	dd gdt_start

bits 32
PModeMain:
	mov ax, DATA_OFFSET
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	mov gs, ax
	mov ebp, 0x9C00
	mov esp, ebp
	
	in al, 2
	out 0x92, al
	
	jmp CODE_OFFSET:KERNEL_START_ADDR

msg_hello: db 'Hello world!', ENDL, 0
msg_disk_error: db 'Disk error occured' ENDL, 0

.hlt:
	hlt
	jmp $

times 510-($-$$) db 0
dw 0AA55h

