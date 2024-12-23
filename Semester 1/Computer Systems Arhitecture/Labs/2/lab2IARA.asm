bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 12h
    b db 2h
    c db 6h
    d dw 100h
    e dw 100h
    f dw 500h
    g dw 100h
    h dw 200h
    

; our code starts here
segment code use32 class=code
    start:
        ; ...
        
       ;11 128*2
        ;mov ax, [a]
        ;mov bl, [b]
        ;mov bh, 0
        ;mul bx
        
       ;22 16/4
        ;mov ax, [a]
        ;mov bl, 4
        ;div bl
                
                ;a,b,c,d - byte
        
       ;5 (c-a-d)+(c-b)-a
        ;mov cl, [c]
        ;mov al, [a]
        ;mov dl, [d]
        ;mov bl, [b]
        
        ;sub cl, al
        ;sub cl, dl
        
        ;mov al, cl
        
        ;mov cl, [c]
        
        ;sub cl, bl
        ;add al, cl
        
        ;sub al, [a]
        
       ;16 a+13-c+d-7+b
        ;mov al, [a]
        ;add al, 13
        ;sub al, [c]
        ;add al, [d]
        ;sub al, 7
        ;add al, [b]
        
                ;a,b,c,d - word
        
       ;9 a-d+b+b+c
        ;mov ax, [a]
        ;sub ax, [d]
        ;add ax, [b]
        ;add ax, [b]
        ;add ax, [c]
        
       ;25 (a+b-c)-d
        ;mov ax, [a]
        ;add ax, [b]
        ;sub ax, [c]
        ;sub ax, [d]
        
                ;a,b,c - byte, d - word
        
       ;7 [100*(d+3)-10]/d
        ;mov ax, 3
        ;add ax, [d]
        ;mov dx, 100
        ;mul dx
        ;push dx
        ;push ax
        ;pop eax
        ;sub eax, 10
        ;div word [d]
       
       ;28 d+10*a-b*c
        mov al, [a]
        mov bl, 10
        mul bl
        add ax, [d]
        mov dx, ax
        mov ax, 0
        mov al, [b]
        mul byte [c]
        sub dx, ax
        mov ax, dx
        mov dx, [d]
        
        *d *d
        
        
                ;a,b,c,d-byte, e,f,g,h-word
       
       ;13 (g+5)-a*d
        ;mov bx, [g]
        ;add bx, 5
        ;mov al, [a]
        ;mul byte [d]
        ;sub bx, ax
        
       ;25 (e+f+g)/(a+b)
        ;mov ax, [e]
        ;add ax, [f]
        ;add ax, [g]
        ;mov bl, [a]
        ;add bl, [b]
        ;div bl
        
        
        
       
       
       
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program

   ;128*2