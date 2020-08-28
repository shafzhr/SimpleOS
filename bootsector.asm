[org 0x7C00] ; Offset of the boot sector
start:

mov bp, 0x8000 ; https://en.wikibooks.org/wiki/X86_Assembly/Bootloaders
mov sp, bp ; Expend the stack because it's placed only 512 beyond the first boot sector

xor dh, dh

push dx ; Drive number
push 0x0000 ; Destination segment to load to
push 0x9000 ; Destination offset to load to
push 0x0002 ; Number of sectors
call load_sectors

cmp ah, 0
jz load_success

push ERR
call print
call new_line
jmp $

load_success:
push 0x9000
call print
call new_line

push 0x9200
call print
call new_line




; push hello_world
; call print
; call new_line

; push welcome
; call print

; call new_line

jmp $

%include "bootsector_print.asm"
%include "disk_loader.asm"

ERR db "ERR", 0

times 510-($-start) db 0
dw 0xaa55 ; Tells the bios that it is bootable. 55aa in little endian(x86).

second_sector db "HELLO!", 0
times 512-($-second_sector) db 0 ; 0 padding

third_sector db "Welcome to my OS.", 0xd, 0xa, "I'm Shachaf the king :P", 0
times 512-($-third_sector) db 0 ; 0 padding
