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
    sir dq 0ABCD0002E7FCh,5,1110111b, 1000000000h
    len equ ($-sir)/8
    x db 69h
    rez times len resd 0
    two dw 2
    three db 3
    current_groups dd 0
    number_of_ones dd 0
    current_double dd 0
    
    format_print dd "%o",0
    

segment code use32 class=code
    start:
    
        
        mov esi, sir
        mov edi, rez
        
        mov ecx,len
        main:
            lodsd ; la inceput , trb sa mutam la sfarsit ; eax = DS:ESI
            mov dword [current_double], eax  ;the initial low word  
            mov edx, eax ;  cu edx shiftam
            shiftari:
                mov ebx, edx ;  cu ebx tinem minte starea inainte de shiftare
                
                mov eax, dword [number_of_ones]
                cmp eax,3
                je este_grupa
                jmp continuare
                
                este_grupa:
                inc dword [current_groups]
                mov dword [number_of_ones],0
                
                continuare:
                
                
                cmp edx,0
                je urm_nr
                                
                
                mov ax, dx
                ;shiftam high wordul pe low word
                shr edx,16
                ; DX:AX =  cuvantul initial
                
                div word [two]
                cmp DX,1 ; comparam sa fie 1 
                je gasit_unu
                mov dword [number_of_ones],0
                jmp shiftare_efectiva                
                gasit_unu:
                inc dword [number_of_ones]
               
                shiftare_efectiva:
                
                mov edx,ebx
                shr edx,1
                jmp shiftari
                         
            
         
        urm_nr:
        lodsd ; la sfarsit    
        
        push dword [current_groups]
        push format_print
        call [printf]
        
        loop main
            
    
    
        push    dword 0      
        call    [exit]       