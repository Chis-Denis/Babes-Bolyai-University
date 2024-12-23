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
    a1 db '256'
    a2 dw 256,256h
    ;a3 dw $+a2
    a4 equ -256/4
    a5 db 256>>1,256<<1
    a6 dw a5-a2,!(a5-a2)
    ;a7 dw [a2],~a2
    a8 dd 256h^256, 256256h
    a9 dd $-a9
    a10 db 256,-255
    a11 dw 256h-256
    a12 dw 256-256h
    a13 dw -256
    a14 dw -256h
    a15 db 2,5,25,6,2,56
    
segment code use32 class=code
    start:
        mov ax,0
    
        
        
        push    dword 0      
        call    [exit]       