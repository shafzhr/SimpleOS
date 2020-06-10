start:
inf_loop:
    jmp inf_loop

times 510-($-start) db 0
dw 0xaa55 ; Tells the bios that it is bootable. 55aa in little endian(x86).
