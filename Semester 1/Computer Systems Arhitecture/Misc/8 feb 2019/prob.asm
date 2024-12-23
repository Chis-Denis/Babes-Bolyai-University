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
    sir dd 1234A678h,12785634h,1A4D3C2Bh
    len equ ($-sir)/4
    sir_nou times 100 dw 0
    num_format db "%d",0

segment code use32 class=code
    start:
        mov esi,sir
        mov ecx, len
        mov edx,0
        mov edi,0
        form:
            lodsw ; high word, we need to take ah
            mov byte [sir_nou+edi],ah
            inc edi
            
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            
            lodsw ;  low word, we need to take ah as well
            mov byte [sir_nou+edi],ah
            inc edi
            
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
            shr ah,1
            adc edx,0
        loop form
            
        push dword edx
        push dword num_format
        call [printf]
        add esp, 4*2
     
        push    dword 0      
        call    [exit]       