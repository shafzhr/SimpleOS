bits 32
print_protected_mode:
    mov edi, 0xB8000 ; video memory
putc:
    mov al, [esi] ; character
    mov ah, 0x0F ; white on black

    cmp al, 0
    je print_pm_done

    mov [edi], ax
    inc esi ; next character
    add edi, 2 ; next place in video memory
    
    jmp putc
print_pm_done:
    ret

clear_screen_protected_mode:
    mov ecx, 80*25 ; screen size
    mov edi, 0xB8000 ; video memory

    mov al, '0' ; character
    mov ah, 0x00 ; black on black
put_clear:
    mov [edi], ax ; little endian => [al(=character), ah(=color)]
    inc esi ; next character
    add edi, 2 ; next place in video memory
    
    loop put_clear
    ret