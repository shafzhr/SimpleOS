[org 0x7C00] ; Offset of the boot sector
start:

push hello_world
call print
call new_line

push welcome
call print

call new_line

jmp $

%include "bootsector_print.asm"

hello_world db "HELLO!", 0
welcome db "Welcome to my OS.", 0

times 510-($-start) db 0
dw 0xaa55 ; Tells the bios that it is bootable. 55aa in little endian(x86).
