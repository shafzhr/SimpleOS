print:
    push bp
    mov bp, sp

    mov bx, [bp+4]
    mov ah, 0x0E

    lprint:
        mov al, [bx]
        cmp al, 0
        jz done ; null terminated string

        int 10h
        
        inc bx
        jmp lprint
    
    done:
        pop bp
    ret 4

new_line:
    xor bh, bh
    mov ah, 0x0E
    
    mov al, 0x0D
    int 10h
    mov al, 0x0A
    int 10h

    ret
