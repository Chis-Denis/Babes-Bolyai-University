bits 32

global start        

extern exit               
import exit msvcrt.dll

segment data use32 class=data

    b db 0xCD
    a dw 0x12
    c dw 0x12
    x dq 0x123456

segment code use32 class=code
    start:
    
     ;c+(a*a-b+7)/(2+a), a-byte; b-doubleword; c-qword unsingned
    
    ;mov al, [a]
    ;mul byte [a]
    ;mov dx, 0
    
    ;mov bx, [b]
    ;mov cx, [b+2]

    ;clc
    ;sub ax, bx
    ;sbb dx, cx
    
    ;xor bx, bx
    ;xor cx, cx
    
    ;mov bl, [a]
    ;add bl, 2
    ;mov bh, 0
    
    ;div bx
    ;mov dx, 0
    
    ;push cx
    ;push bx
    ;pop ebx
    ;mov ecx, 0
    
    ;add ebx, [c]
    ;adc ecx, [c+4]
    
    
     ;x-(a*100+b)/(b+c-1); a-word; b-byte; c-word; x-qword
    
    mov ax, [a]
    mov bx, 100
    mul bx
    mov cx, 0
    add ax, bx
    adc dx, cx
    
    xor bx, bx
    xor cx, cx
    
    mov bl, [b]
    mov bh, 0
    add bx, [c]
    sub bx, 1
    
    div bx
    
    xor bx, bx
    xor cx, cx
    
    mov dx, 0
    push dx
    push ax
    pop eax
    
    mov ebx, [x]
    mov ecx, [x+4]
    
    sub ebx, eax
    sbb ecx, edx
    
        push    dword 0
        call    [exit]
