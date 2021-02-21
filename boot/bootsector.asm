[org 0x7C00] ; Offset of the boot sector

KERNEL_OFFSET equ 0x1000

start:
    mov [BOOT_DRIVE_NUMBER], dl

    mov bp, 0x9000 ; https://en.wikibooks.org/wiki/X86_Assembly/Bootloaders
    mov sp, bp ; Expend the stack because it's placed only 512 beyond the first boot sector

    push ENTER_16BIT
    call print
    call new_line

    call load_kernel

    cmp ah, 0
    jz kernel_load_success
    
    push ERR
    call print
    call new_line
    jmp $

kernel_load_success:
    call print
    call new_line

    call switch_to_protected_mode
    jmp $ ; will not be executed

load_kernel:
    xor dh, dh 
    mov dl, [BOOT_DRIVE_NUMBER]
    
    push dx ; Drive number
    push 0x0000 ; Destination segment to load to
    push KERNEL_OFFSET ; Destination offset to load to
    push 30 ; Number of sectors
    call load_sectors
    ret


%include "boot/bootsector_print.asm"
%include "boot/disk_loader.asm"
%include "boot/gdt/gdt.asm"
%include "boot/protected_mode/switchPM.asm"
%include "boot/protected_mode/printPM.asm"

bits 32
main_PM:
    call clear_screen_protected_mode
    mov esi, PM_SUCCESS
    call print_protected_mode
    call KERNEL_OFFSET
    jmp $


ERR db "ERR", 0
BOOT_DRIVE_NUMBER db 0
ENTER_16BIT db "Entered 16-bit", 0
PM_SUCCESS db "Successfully entered protected mode", 0

STATUS_PRINT db "0", 0

times 510-($-start) db 0
dw 0xaa55 ; Tells the bios that it is bootable. 55aa in little endian(x86).