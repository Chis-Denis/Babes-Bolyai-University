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
    sir dd 1234A678h,12345678h,1AC3B47Dh,0FEDC9876h
    len equ ($-sir)/4
    suma db 0
    sir_pozitie times 100 resb 0    
    byte_maxim db 0
    pozitie dd 0    
    format_hexa db "%X",0
    format_suma db "%d",0
    space db " ",0
    lungime_sir db 0

segment code use32 class=code


    start1:
        mov eax,[esp+8]
        mov edx, [esp+4] ;edx suma
        mov ebx,0
        
        mov ecx,4
        repeta:
            inc ebx
            cmp al,byte [byte_maxim]
            ja mai_mare
            jmp shiftare
            
            mai_mare:
            mov byte [byte_maxim],al
            mov dword [pozitie],ebx
            jmp shiftare
        
            shiftare:
                shr eax,8
        loop repeta
        
        mov al, byte [byte_maxim]
        add edx, dword [byte_maxim]
        mov byte [byte_maxim],0
        mov dword [pozitie],0
        
        ret 4*2
    start:
        mov cl, len
        mov byte [lungime_sir],cl
        
        mov esi, sir
        mov edi,0
        parcurg: 
            
            lodsd
            
            push eax
            push dword [suma]
            call start1
            
            ;eax byteul maxim
            ;ebx pozitia
            
            mov byte [suma],dl
            mov byte [sir_pozitie+edi],bl
            inc edi
            
            push dword eax
            push dword format_hexa
            call [printf]
            add esp, 4*2
            
            push dword space
            call [printf]
            add esp, 4
            
            
            dec byte [lungime_sir]
            mov cl,byte [lungime_sir]
            cmp cl,0
            je final        
        
        jmp parcurg 
        
                
    final:   
        push dword [suma]
        push format_suma
        call [printf]
        add esp, 4*2
        
        
        push dword sir_pozitie
        call [printf]
        add esp, 4*1

    
        push    dword 0      
        call    [exit]       