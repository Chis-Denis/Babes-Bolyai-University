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
    a1 dd -103,a6
    a2 dw 1&2,3|4,5^6,1|2,3^4,5&6
       dw 1^2,3&4,5|6
    a3 dw $-$$,$$-a3
    a4 db -((-1)^0bbh),1^0bbh
    a5 dd -129<<1Fh,81h>>0111b
    a6 dd "a1a2a3a4a5",(a6-a5)<<(a5-a4)
    a7 times 4 dw a2,a2+1
    a8 dw !(a2-a1)    
    a db -1
    b dw 80h


segment code use32 class=code
    start:
        ;movsx ebx, [ebx+esp]6
        ;xchg ebx, [ebx+esp]
        ;lea eax, [ebx+esp]
        ;lea eax, [ebx+ebp]
        ;mov ax, a+b 
        
        ;mov ax,0100h
        ;mov bx, 1000+10b
        ;idiv bl
        
        ;mov ah,0cdh
        ;mov al,0ebh
        ;add ah,al
        
        ;mov ax, 1010h
        ;mov bx,1111b
        ;mul bl
        
        mov dh,200
        mov ch,62h
        sub dh,ch
        
     
        push    dword 0      
        call    [exit]       