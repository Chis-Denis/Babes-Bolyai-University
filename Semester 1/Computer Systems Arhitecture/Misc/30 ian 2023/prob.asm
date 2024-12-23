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
    s1 dq 0x1234613456785678,0x9234123456785678,0x1234123410785678,0x1234613498769876
    len equ ($-s1)/8
    s2 times 100 dd 0
    number_format db "%d",0
    
    

segment code use32 class=code

    vocale:
        mov esi,[esp+4]
        mov ecx,0
        ;in ecx we save the number of doublewords with the prop that the high byte of the low word is the ascii for a vowel
        check_double:
            lodsd ; we need to check ah
            
            cmp eax,0
            je final
            
            cmp ah,"a"
            je vocala
            cmp ah,"e"
            je vocala
            cmp ah,"i"
            je vocala
            cmp ah,"o"
            je vocala
            cmp ah,"u"
            je vocala
            jmp check_double
            
            vocala:
            inc ecx
            jmp check_double
            
    
    final:
    ret
    
    
    start:
        mov esi,s1
        mov edi,0
        mov ecx, len
        formeaza_s2:
            lodsd ; eax low word 
            
            mov ebx, eax ; ebx = eax
            
            lodsd ; eax =  high word
            
            cmp eax,ebx
            jg este_mai_mare
            jmp continue_loop
            
            este_mai_mare:
            mov dword [s2+edi],eax
            add edi,4
            
            continue_loop:
        loop formeaza_s2
        
        push dword s2
        call vocale
        
        
        push dword ecx
        push dword number_format
        call [printf]
        add esp, 4*2
        
            
    
    
        push    dword 0      
        call    [exit]       