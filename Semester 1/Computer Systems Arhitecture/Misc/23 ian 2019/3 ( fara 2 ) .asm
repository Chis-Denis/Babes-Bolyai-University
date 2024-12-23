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
    

segment code use32 class=code
    start:
        ; mov ah,129
        ;mov bh,97h
        ;add ah,bh
        
        ;mov ax,128
        ;sar al,7
        ;imul ah
        
        ;mov ax,256
        ;mov bx, -1
        ;add ah,bh
        
        ;mov ah,128|2
        ;mov bh,90h>>3
        ;sub ah,bh
        
        ;lea esi,[esp+4]
        ;lea edi,[esi-8]
        ;push esp
        ;ss lodsd
        ;ss stosd
        
        ;push dword [esp+4]
        
        ;xor eax,eax
        ;lea ebx,[esi]
        ;lodsb <-> es xlat
        
        ;mov eax,-2
        ;mov ebx,-1
        ;div bl
        
        ;mov eax,65409
        ;idiv ah
        ;add al,al
        
        ;mov eax, 255h&255   
        ;mov ebx, 256^256h
        ;mul bh
        
        mov ax,128|2
        mov bh, 4ah>>2
        sub ah,bh
        
        push    dword 0      
        call    [exit]       