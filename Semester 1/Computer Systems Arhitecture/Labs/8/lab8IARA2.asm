bits 32
global start        

extern exit, printf                 
import exit msvcrt.dll    
import printf msvcrt.dll    

;A negative number a (a: dword) is given.
; Display the value of that number in base 10 and in the base 16 in the following format:
; "a = <base_10> (base 10), a = <base_16> (base 16)"

segment data use32 class=data
     a dd 0xFFFFAB12
     formatdeca db "a = %d (base 10)", 0
     formathexa db "a = %X (base 16)", 0

segment  code use32 class=code
    start:
        push dword [a]
        push dword formathexa
        call [printf]       
        add esp, 4 * 2    
        

        push dword [a]
        push dword formatdeca
        call [printf]       
        add esp, 4 * 2    
        
        push dword 0   
        call [exit]