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
    s1 dw 12ABh,34CDh,56EFh
    len equ ($-s1)/2
    s2 dw 3500h,0011h,7FFBh
    sir_nou times 100 dd 0
    pos dd 0
    

segment code use32 class=code
    start:
        mov ecx, len
        mov edi,0
        jecxz end
        form:
            
            mov esi,s1
            add esi, dword [pos]
            lodsw
            mov bx,ax

            mov esi,s2
            add esi, dword [pos]
            lodsw 

            add dword [pos],2
        
        loop form
            
        
    
        end:
    
        push    dword 0      
        call    [exit]       