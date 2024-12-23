bits 32 
global start        

extern exit,fopen,printf,scanf,fread,fclose,fprintf,gets          
import exit msvcrt.dll   
import fopen msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll
import gets msvcrt.dll

segment data use32 class=data
    a1 db '256,-256'
    a2 dw 256, 256h
    a3 dw $-a2
    a4 equ -256/4
    a5 db 128>>1, -128<<1
    a6 dw a2-a5,~(a2-a5)
    ;a7 dd [a2],!a2
    a8 dd 256h^256,256256h
    a9 dd ($-a8)+(a10-$)
    a10 dw -255,256
    a11 resb 6
    a12 times 4 dw 256
    ;a13 dw times 4 -128
    times 2 resw 2
    times 2 dd 12345678h
    
    x db 1

segment code use32 class=code
    start:
        ;cld
        ;push ax
        ;mov ax,[esp+2]
        ;stosw
        ;lea edi, [edi-2]
        ;add esp,4
        
        ;mov word [edi],a ; ?
        mov ebx, 0x12345678
        
        ;lea ebx, [ebx+6] ; 1
        ;lea ebx, [bx+6]  ; 2
        ;lea bx,[bx+6]  ;1
        ;lea bx, [ebx+6] ;1
        ;mov ebx, ebx+6 ;3
        ;mov ebx, [ebx+6] ;4
        ;movzx ebx, [ebx+6] ; 3
        ;movzx ebx, [bx+6] ; 3
        
        add bx,6 ; 1
        movzx ebx,bx
        
        ;mov [ebx], dword [bx+6] ; 3
        ;add ebx,6 ; 1
        ;add bx, 6 ; 1
        ;push [ebx+6] ; pop ebx , 3
        ;xchg ebx, [ebx+6] ; 4
        

  
        push    dword 0      
        call    [exit]       