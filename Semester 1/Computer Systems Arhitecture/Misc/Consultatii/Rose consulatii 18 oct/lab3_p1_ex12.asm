bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...(a+b+d)-(a-c+d)+(b-c)
    ;a - byte, b - word, c - double word, d - qword
    a db 2
    b dw 10
    c dd 12
    d dq 20
    
; our code starts here
segment code use32 class=code
    start:
        ; ...UNSIGNED
       
        mov al,[a]
        mov ah,0
        add ax,[b] ;ax=a+b
        mov bx,ax
        mov eax,0
        mov ax,bx ;eax=a+b
        mov edx,0 ; edx:eax=a+b
        
        add eax ,[d]
        adc edx,[d+4] ; edx:eax=a+b+d 
        
        mov ecx,0
        mov cl,[a]
        sub ecx,[c] ; ecx=a-c
         
        mov ebx,0 ; ebx:ecx=a-c
        
        add ecx,[d]
        adc ebx,[d+4] ;ebx:ecx=a-c+d
        
       
        sub eax,ecx
        sbb edx,ebx  ; edx:eax=(a+b+d)-(a-c+d)
        
        mov ebx,0
        mov bx,[b]
        
        sub ebx,[c] ; ebx=b-c
        
        add eax,ebx
        adc edx,0 ; edx:eax=(a+b+d)-(a-c+d)+(b-c)
        
         ; ...(a+b+d)-(a-c+d)+(b-c)
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
