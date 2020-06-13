print:
    push bp
    mov bp, sp
    
    push ax
    push bx

    mov bx, [bp+4]
    mov ah, 0x0E

    .lprint:
        mov al, [bx]
        cmp al, 0
        jz .done ; null terminated string

        int 10h
        
        inc bx
        jmp .lprint
    
    .done:
        pop bx
        pop ax    
    pop bp
    ret 4

new_line:
    push ax
    push bx

    xor bh, bh
    mov ah, 0x0E
    
    mov al, 0x0D
    int 10h
    mov al, 0x0A
    int 10h

    pop bx
    pop ax
    ret
