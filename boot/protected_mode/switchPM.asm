; Switch to protected mode
bits 16
switch_to_protected_mode:
    cli                   ; disable interrupts
    lgdt [gdt_descriptor] ; load gdt
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_SEG:pm_mode

bits 32
pm_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax

    mov ebp, 0x90000 ; set the stack up a lot
    mov esp, ebp

    jmp main_PM