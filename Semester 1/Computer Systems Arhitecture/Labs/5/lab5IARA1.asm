bits 32

global start        

extern exit               
import exit msvcrt.dll

segment data use32 class=data

    set db '+', '4', '2', 'a', '@', '3', '$', '*'
    lenghten equ $-set

    ;good_set db '!', '@', '#', '$', '%', '^', '&', '*'
    ;lenghten_good equ $-good_set
    
    destination times lenghten db 0

segment code use32 class=code

    ;Given a character string S, obtain the string D containing all special characters (!@#$%^&*) of the string S.
    ;Example:
    ;S: '+', '4', '2', 'a', '@', '3', '$', '*'
    ;D: '@','$','*'
    
    
    start:
    
    mov ecx, lenghten
    mov esi, 0
    mov edi, 0
    
    main_loop:
    
        mov al, [set+esi]
        
        cmp al, '!'
        je store_char
        cmp al, '@'
        je store_char
        cmp al, '#'
        je store_char
        cmp al, '$'
        je store_char
        cmp al, '%'
        je store_char
        cmp al, '^'
        je store_char
        cmp al, '&'
        je store_char
        cmp al, '*'
        je store_char
        cmp al, '+'
        je store_char
        
    next_element:
        
        inc esi
        dec ecx
        jnz main_loop
        
        jmp endloop
        
    store_char:
    
        mov [destination + edi], al
        inc edi
        jmp next_element
    
    endloop:

    
        push    dword 0
        call    [exit]