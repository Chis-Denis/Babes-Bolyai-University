bits 32
global start        

extern exit,fopen,printf,fread,fclose        
import exit msvcrt.dll   
import fopen msvcrt.dll
import printf msvcrt.dll
import fread msvcrt.dll 
import fclose msvcrt.dll

;A text file is given. Read the content of the file, count the number of vowels and display the result on the screen. The name of text file is defined in the data segment.

segment data use32 class=data
    file_mode db "r",0
    file_input_name db "input.txt",0
    file_input_descriptor dd -1
    number_of_appeareances dd 0
    format db "Number of vowels: %d", 10, 0
    file_name db "lab8IARA3.txt", 0
    file_text db 0

segment  code use32 class=code
    start:
    
        push dword file_mode
        push dword file_name
        call [fopen]
        add esp, 4*2
        mov [file_input_descriptor], eax
        cmp eax, 0
        je file_not_found
 
        
        read:
        
            push dword [file_input_descriptor]
            push dword 1
            push dword 1
            push dword file_text
            call [fread]
            add esp, 4*4
            
            cmp eax, 0
            je final
            
            mov dl, [file_text]         
  
                mov al, dl
                or al, 0x20
                cmp al, 'a'
                je is_vowel_yes
                cmp al, 'e'
                je is_vowel_yes
                cmp al, 'i'
                je is_vowel_yes
                cmp al, 'o'
                je is_vowel_yes
                cmp al, 'u'
                je is_vowel_yes
                
                jmp read               

            is_vowel_yes:
                inc dword [number_of_appeareances]
                jmp read
                
            
        final:
        
        
        push dword [file_input_descriptor]
        call [fclose]
        add esp, 4*1
        
        push dword [number_of_appeareances]
        push dword format
        call [printf]
        add esp, 4*2
        
        
        push dword 0
        call [exit]
    
    file_not_found:
        
        push dword 0   
        call [exit]