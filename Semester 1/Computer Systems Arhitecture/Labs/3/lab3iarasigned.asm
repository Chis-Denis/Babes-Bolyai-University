bits 32

global start        

extern exit               
import exit msvcrt.dll

segment data use32 class=data

    a dw 0x1122
    b db 0x23
    c dw 0x1234
    x dq 0x12345678

segment code use32 class=code
    start:
    
     ;c+(a*a-b+7)/(2+a), a-byte; b-doubleword; c-qword signed
     
    ;mov al, [a]
    ;imul byte [a]
    ;cwd
    ;
    ;mov bx, [b]
    ;mov cx, [b+2]

   ;sub ax, bx
    ;sbb dx, cx
    
    ;mov bx, [a]
    ;add bx, 2
    
    ;idiv bx
    ;cwd
    
    ;push dx
    ;push ax
    ;pop eax
    ;   cdq
    
    ;add eax, [c]
    ;adc edx, [c+4]
    
     ;x-(a*100+b)/(b+c-1); a-word; b-byte; c-word; x-qword signed
    
    mov ax, [a]
    mov bx, 100
    imul bx
    
    add ax, [b]
    adc dx, [b+2]
    
    xor bx, bx
    xor cx, cx
    
    mov bx, ax
    mov dx, cx
    
    xor ax, ax
    xor dx, dx
    
    mov al, [b]
    cbw
    add ax, [c]
    sub ax, 1
    
    push dx
    push ax
    mov ax, bx
    mov dx, cx
    pop eax
    xor cx, cx
    
    idiv bx
    
    xor bx, bx
    xor cx, cx
    
    cwd
    push dx
    push ax
    pop eax
    
    mov ebx, [x]
    mov ecx, [x+4]
    
    sub ebx, eax
    sbb ecx, edx
     
        push    dword 0
        call    [exit]
