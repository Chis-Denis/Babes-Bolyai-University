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
    sir dq 1122334455667788h,99AABBCCDDEEFF11h,0xFF00000000000055,0xFFFEFFFFFFFFEFFF,0x450000AB000000AB,0x11113733555577AB
    len equ ($-sir) ; we want the len in bytes 
    n dd 0
    n_format db "%d",0
    sir_nou times 100 db 0
    sir_nou_len dd 0
    second_for_len dd 0
    print_len db 0
    first_byte_0 db 0
    second_byte_0 db 0
    space db " ",0
    hexa_format db "%X",0
    string_format db "%s",0

    
segment code use32 class=code
    start:
        push dword n
        push dword n_format
        call [scanf]
        add esp, 4*2
        
        mov eax, dword [n]
        cmp eax,6
        ja not_in_range
        
        cmp eax,1
        jb not_in_range
              
        
        mov esi,sir
        mov edi,sir_nou
        
      
        mov ebx,0
        mov ecx,len
        transfer:
            movsb 

            inc ebx
            mov edx,dword [n]
            dec edx
            
            cmp ecx,0
            je afara
            
            count:
                lodsb
                dec edx
                dec ecx
                cmp ecx,0
                je afara
                cmp edx,0
                je continua_loop
                jmp count
                
        continua_loop:
        loop transfer
        
        afara:
        mov dword [sir_nou_len],ebx ; lungimea sirului trb sa fie cu 1 mai mica ca sa putem face bubble sort
        mov dword [second_for_len],ebx
        mov dword [print_len],ebx
   
        
        
        ; so far we have transfered the bytes from one register to another
        
        
        oloop:
            mov ecx, dword [second_for_len]
            mov esi, sir_nou

                iloop:
                    mov al, [esi]
                    mov bl,0 ; in bl we coun the bits
                    
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    
                    mov byte [first_byte_0],bl
                    
                    mov al, [esi+1]
                    mov bl,0 ; in bl we coun the bits
                    
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    shr al,1
                    adc bl,0
                    
                    mov byte [second_byte_0],bl
                    
                    mov al,byte [first_byte_0]
                    mov bl,byte [second_byte_0]
                    
                    cmp al,bl
           
                    jg common1   
                    mov al,[esi]                    
                    xchg al, [esi+1]
                    mov [esi], al                    

                    common1:
                        inc esi
                        loop iloop

        dec dword [sir_nou_len]
        jnz oloop
        
        xor eax,eax
        mov esi,sir_nou
        xor ecx,ecx
        inc byte [print_len]
             
        
        printeaza:
            lodsb 
            
            push dword eax
            push dword hexa_format
            call [printf]
            add esp, 4*2
            

            push dword space
            push dword string_format
            call [printf]
            add esp,4*2
            
            dec byte [print_len]
            jnz printeaza
            
          
        not_in_range:
        push    dword 0      
        call    [exit]       