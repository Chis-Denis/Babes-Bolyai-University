bits 32
global start        

extern exit, fopen, fread, fprintf, fclose        
import exit msvcrt.dll   
import fopen msvcrt.dll
import fprintf msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll

;A file name and a text (defined in the data segment) are given. The text contains lowercase letters, uppercase letters, digits and special characters. Replace all the special characters from the given text with the character 'X'. Create a file with the given name and write the generated text to file.
segment data use32 class=data
    file_mode db 'w',0
    file_output_name db "input.txt",0
    file_output_descriptor dd -1
    number_of_appeareances dd 0
    format db "The text transformed: %s", 0
    file_name db "lab8IARA4.txt", 0
    file_input_text db "!ow! $ Cox?", 0
    len equ $-file_input_text
    file_output_text times len db 0

segment  code use32 class=code
    start:
    
        push dword file_mode
        push dword file_name
        call [fopen]
        add esp, 4*2
        mov [file_output_descriptor], eax
        
        mov esi, 0
        mov edi, 0
        cld
        
    repeta:
        mov al, [file_input_text + esi]
        cmp al, 0
        jz final
        
        cmp al, 'a'
        jb change_to_X
        cmp al, 'z'
        jmp copy_char

    check_digit:
        cmp al, '0'
        jb change_to_X
        cmp al, '9'
        ja change_to_X
        jmp copy_char

    change_to_X:
        mov al, 'X'

    copy_char:
        mov [file_output_text + edi], al
        inc esi
        inc edi
        jmp repeta

    final:
        mov byte [file_output_text + edi], 0
        
        push dword file_output_text
        push dword format
        push dword [file_output_descriptor]
        call [fprintf]
        add esp, 4*3
        
        
        
        push dword [file_output_descriptor]
        call [fclose]
        add esp, 4*1
        
        
        push dword 0
        call [exit]