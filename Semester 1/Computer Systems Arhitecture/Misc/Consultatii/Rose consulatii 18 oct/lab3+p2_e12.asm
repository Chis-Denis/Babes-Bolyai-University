bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...(a-b-c)+(d-b-c)-(a-d)
    ;a - byte, b - word, c - double word, d - qword - Signed representation
    a db 2
    b dw 3
    c dd 10
    d dq 12
    r dd 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov al,[a]
        cbw ; ax=a 
        sub ax,[b] ;ax=a-b
        cwde ; eax=a-b
        sub eax,[c] ;eax=a-b+c 
         
        mov [r],eax ; r =a-b-c
        mov ax,[b]
        cwde ; eax=[b]
        cdq ; edx:eax=b 
        
        mov ebx,[d]   ;low  part
        mov ecx,[d+4] ; high part
        
        sub ebx,eax
        sbb ecx,edx ; ecx:ebx=d-b 
         
        mov eax,[c]
        cdq ; edx:eax=c  
        
        sub ebx,eax
        sbb ecx, edx ; ecx:ebx=d-b-c 
        
        mov eax,[r]
        cdq ; edx:eax= r 
        
        add eax,ebx
        adc edx,ecx ; edx:eax=(a-b-c)+(d-b-c)
        
        mov ebx,eax ; edx:ebx=(a-b-c)+(d-b-c)
        mov ecx,edx ; ecx:ebx=(a-b-c)+(d-b-c)
        mov al,[a]
        cbw ;ax=a 
        cwde ;eax=a 
        cdq ; edx:eax=a 
        sub eax,[d]
        sbb edx,[d+4] ;edx:eax=a-d
        
        sub ebx,eax
        sbb ecx,edx ;ecx:ebx= (a-b-c)+(d-b-c)-(a-d)
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
