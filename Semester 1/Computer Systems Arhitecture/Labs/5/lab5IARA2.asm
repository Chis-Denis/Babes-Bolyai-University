bits 32

global start        

extern exit               
import exit msvcrt.dll

segment data use32 class=data

    first_set db 1, 3, 6, 2, 3, 2
    lenghten_first equ $-first_set

    second_set db 6, 3, 8, 1, 2, 5
    lenghten_second equ $-second_set
    
    destination times (lenghten_first + lenghten_second) db 0

segment code use32 class=code

    ;Two byte strings S1 and S2 of the same length are given. Obtain the string D where each element is the difference of the corresponding elements from S1 and S2
    ;Example:
    ;S1: 1, 3, 6, 2, 3, 2
    ;S2: 6, 3, 8, 1, 2, 5
    ;D: -5, 0, -2, 1, 1, -3
    
    
    start:
    
    mov ecx, lenghten_first
    mov esi, 0
    mov edi, 0
    
    main_loop:
    
        mov al, [first_set+esi]
        sub al, [second_set+esi]
        
        mov [destination + edi], al
        
        
        inc esi
        inc edi
        dec ecx
        
        jnz main_loop        
        
    endloop:

    
        push    dword 0
        call    [exit]