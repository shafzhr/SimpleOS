bits 32
main_PM:
    call clear_screen_protected_mode
    mov esi, PM_SUCCESS
    call print_protected_mode
    jmp $

PM_SUCCESS db "Successfully entered protected mode", 0