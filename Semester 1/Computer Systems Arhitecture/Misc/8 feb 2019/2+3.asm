bits 32 
global start        

extern exit,fopen,printf,scanf,fread,fclose,fprintf,gets          
import exit msvcrt.dll   
import fopen msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll
import fread msvcrt.dll
import fclose msvcrt.dll
import fprintf msvcrt.dll
import gets msvcrt.dll

segment data use32 class=data
    x dw -256,256h
    y dw 256|-256,256h&256
    z db $-z,y-x
      db 'y'-'x','y-x'
    a db 512>>2,-512<<2
    b dw z-a,!(z-a)
    c dd ($-b) + (d-$)
    d db -128,128^(~128)
    e times 2 resw 6
      times 2 dd 1234h,5678h
    
segment code use32 class=code
    start:
        ;mov al,128
    
        ;mov bh,7fh
        ;cmp bh,al
        ;rcr ah,1
        ;sar ah,7
        
        ;movsx ax,al
        
        ;mov eax,200
        ;mov ebx, 254h
        ;idiv bl
        
        ;mov ax,256h
        ;mov dx,-1
        ;add ah,dh
        
        ;mov ax,~(16h|32)
        ;mov bx,2000h>>4
        ;imul bh
        
        ;mov ax,21<<7
        ;mov bh,10h^3
        ;sub bh,al
        
        
        mov eax,12345678h
        mov ebx,87654321h
        
        shl bh,8 ;  bh = 0 
        add bx,ax ; 
        lea bx, [bx+al]
        
        
    
        push    dword 0      
        call    [exit]       