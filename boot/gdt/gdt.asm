; refer https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf
; and https://wiki.osdev.org/Global_Descriptor_Table
; for GDT structure
gdt_start:

gdt_null:
    dq 0

gdt_code_segment:
    dw 0xffff ; limit (0-15)
    dw 0x0000 ; base (0-15)
    db 0 ; base (16-23)
    db 10011000b ; access byte
    db 11001111b ; flags and limit(16-19)
    db 0 ; base (24-31)

gdt_data_segment:
    dw 0xffff ; limit (0-15)
    dw 0x0000 ; base (0-15)
    db 0 ; base (16-23)
    db 10010010b ; access byte
    db 11001111b ; flags and limit(16-19)
    db 0 ; base (24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code_segment - gdt_start
DATA_SEG equ gdt_data_segment - gdt_start