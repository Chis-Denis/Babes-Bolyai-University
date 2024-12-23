bits 32

global start        

extern exit, printf, scanf
import exit msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll

segment data use32 class=data
    console_input_a dd 0
    console_input_b dd 0
    message db "Enter a number: ", 0
    output_msg db "Number read is %d", 10, 0
    format db "%d", 0

segment code use32 class=code
    start:
        ;Read two numbers a and b (in base 10) from the keyboard and calculate their product. This value will be stored in a variable called "result" (defined in the data segment).
    
        ; Print prompt message
        push dword message
        call [printf]
        add esp, 4*1

        ; Read a number from the console
        push dword console_input_a
        push dword format
        call [scanf]
        add esp, 4*2
        
        ; Print prompt message
        push dword message
        call [printf]
        add esp, 4*1

        ; Read a number from the console
        push dword console_input_b
        push dword format
        call [scanf]
        add esp, 4*2
        
        mov ax, [console_input_a]
        mov bx, [console_input_b]
        mul bx
        mov [console_input_a], eax
        

        ; Print the number read
        push dword [console_input_a]
        push dword output_msg
        call [printf]
        add esp, 4*2
        

        ; Exit the program
        push dword 0
        call [exit]
