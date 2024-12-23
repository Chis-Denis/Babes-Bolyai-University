bits 32

global start        

extern exit               
import exit msvcrt.dll

segment data use32 class=data

    a dw 0b1010_1010_1010_1010
    b dw 0b1111_1111_1111_1110
    
    c dd 0b0000_0000_0000_0000_0000_0000_0000_0000

segment code use32 class=code
    start:
    
        ;Given the words A and B, compute the doubleword C as follows:
        ;the bits 0-4 of C are the same as the bits 11-15 of A
        ;the bits 5-11 of C have the value 1
        ;the bits 12-15 of C are the same as the bits 8-11 of B
        ;the bits 16-31 of C are the same as the bits of A
    
        ;mov ax, [a]
        ;mov bx, [b]
        
        ;mov cx, ax
        ;and cx, 0Fh
        
        
        ;mov dx, 0b0000_1111_1110_0000
        ;or cx, dx
        ;xor dx, dx
        
        
        ;shr bx, 8
        ;shl bx, 12
        ;or cx, bx
        ;xor bx, bx
        ;or bx, cx
        ;mov cx, 0
        
        
        ;or cx, ax
        ;push cx
        ;push bx
        ;pop ebx
        
        
        ;Given the word A, compute the doubleword B as follows:
        ;the bits 28-31 of B have the value 1;
        ;the bits 24- 25 and 26-27 of B are the same as the bits 8-9 of A
        ;the bits 20-23 of B are the invert of the bits 0-3 of A ;
        ;the bits 16-19 of B have the value 0
        ;the bits 0-15 of B are the same as the bits 16-31 of B.
        
        mov ax, [a]
        
        mov bx, 0
        mov cx, 0
        
        
        mov dx, 0b1111_0000_0000_0000
        or cx, dx
        xor dx, dx
        
        
        shl ax, 6
        shr ax, 14
        or dx, ax
        shl ax, 2
        or dx, ax
        shl dx, 8
        or cx, dx
        
        
        xor ax, ax
        mov ax, [a]
        xor dx, dx
        and ax, 0Fh
        not ax
        shl ax, 12
        shr ax, 8
        or cx, ax
        
        
        or bx, cx
        push cx
        push bx
        pop ebx
        
        
        push    dword 0
        call    [exit]