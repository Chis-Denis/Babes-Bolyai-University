bits 32

global start        

extern exit               
import exit msvcrt.dll

segment data use32 class=data

    a db 0xCD
    b dw 0x234
    c dd 0x12344
    d dq 0x123456789ABCD

segment code use32 class=code
    start:
    
        mov al, 1
        mov al, 2
        mov al, 0
        mov al, 255
        add al, -1
    
         ;a - byte, b - word, c - double word, d - qword - Unsigned representation
        ;(b+b)+(c-a)+d
        mov bx, [b]
        add bx, [b]
        
        mov bx, [c]
        mov cx, [c+2]
        
        mov al, [a]
        mov ah, 0
        mov dx, 0
        
        sub bx, ax
        sbb cx, dx
        
        xor ax, ax
        xor dx, dx
        
        mov ax, [b]
        add ax, [b]
        mov dx, 0
        
        add ax, bx
        adc dx, cx
        
        push dx
        push ax
        pop eax
        mov edx, 0
        
        add eax, [d]
        adc eax, [d+4]
        
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx
        
        ;(d+d)-a-b-c
        
        mov al, [a]
        mov ah, 0
        
        sub ax, [b]
        mov dx, 0
        
        sub ax, [c]
        sbb dx, [c+2]
        
        push dx
        push ax
        pop eax
        mov edx, 0
        
        mov ebx, [d]
        mov ecx, [d+4]
        add ebx, [d]
        adc ecx, [d+4]
        
        sub ebx, eax
        sbb ecx, edx
        
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx
    
         ;a - byte, b - word, c - double word, d - qword - Signed representation
        ;c-b-(a+a)-b
        
        mov bx, [c]
        mov cx, [c+2]
        
        mov ax, [b]
        cwd

        sub bx, ax
        sbb cx, dx
        sub bx, ax
        sbb cx, dx
        
        xor ax, ax
        xor dx, dx
        
        mov al, [a]
        add al, [a]
        
        cbw
        cwd
        
        sub bx, ax
        sbb cx, dx
        
        push cx
        push bx
        pop ebx
        
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx
        
        ;c+a+b+b+a
        
        mov bx, [c]
        mov cx, [c+2]
        
        mov al, [a]
        cbw
        cwd
        add bx, ax
        adc cx, dx
        
        xor ax, ax
        xor dx, dx
        
        mov ax, [b]
        cwd
        
        add bx, ax
        adc cx, dx
        add bx, ax
        adc cx, dx
        
        xor ax, ax
        xor dx, dx
        
        mov al, [a]
        cbw
        cwd
        add bx, ax
        adc cx, dx
        
        push cx
        push bx
        pop ebx
        
        push    dword 0
        call    [exit]
