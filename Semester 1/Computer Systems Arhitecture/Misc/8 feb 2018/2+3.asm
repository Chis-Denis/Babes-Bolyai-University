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
    read_mode db "r",0
    write_mode db "w",0
    file_input_name db "input.txt",0
    file_output_name db "output.txt",0
    file_input_descriptor dd -1
    file_output_descriptor dd -1
    number_format db "%d",0
    string_format db "%s",0
    new_line db 10,0
    new_line_format db "%s",0

segment code use32 class=code
    start:
    
        ;mov ax, 1000h
        ;mov bl,1000b+10b
        ;div bl
        
        ;mov ax,1001h
        ;mov bx,1111b
        ;imul bl
        
        mov dh,62h
        mov ch, 200
        sub dh,ch
        
        
        
        
        
        
    

        push    dword 0      
        call    [exit]       