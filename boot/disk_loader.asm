; [bp + 10] - Drive number
; [bp + 8] - Destination segment
; [bp + 6] - Destination offset
; [bp + 4] - Amount of sectors
load_sectors:
    push bp
    mov bp, sp

    mov dl, [bp + 10] ; drive number
    mov ax, [bp + 8] ; Dest segment
    mov es, ax
    mov bx, [bp + 6] ; Dest offset
    mov al, [bp + 4] ; Amount of sectors byte

    mov cl, 2 ; from sector
    xor ch, ch ; cyl number
    xor dh, dh ; head number

    mov ah, 2
    int 13h
    jnc .read_success
    
    push read_error
    call print
    call new_line
    jmp .done

    .read_success:
        cmp al, [bp + 4] ; al contains the amount of sectors that were read
        jz .done

        push partly_read
        call print
        call new_line
        jmp .done
    .full_success:
        push success_msg
        call print
        call new_line
    
    .done:
        pop bp

    ret 8


read_error db "There was an error loading the sectors", 0
partly_read db "Couldn't read all the sectors", 0
success_msg db "Successfully loaded the sectors", 0
