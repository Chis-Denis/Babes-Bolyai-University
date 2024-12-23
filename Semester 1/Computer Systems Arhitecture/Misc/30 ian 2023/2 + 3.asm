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
    ;x dw -129, 10+100h+1000b
    ;y dw 1001h>>1001b, 128h & 128
    ;z dw z, $$-z
    ;w dd x+y-z,w-y+x
    ;h dw 101b, 11h-11b,h-11
    ;a db $$-$
    ;b dd  2Ch,h-b+0ah-0bh
   ; c db z-w
    ;d dw -513,128^(~128)
    ;e dd 'abcdefh'
   ; f dw w-1
   ; g times 3 dw 'db'
  ;  k dw c+0ch
   ; m dd a+0ah
    ;s dd start-start1
  ;  xx dd 1abcdh,0abcdh
segment code use32 class=code
    start:
        ;mov ax, 128|2
        ;mov bh, 4ah>>2
        ;sub ah,bh
        
        ;mov ebx,12345678h       
        ;xor ah,ah
        ;cwde
        ;add ebx,eax
        ;mov al,[ebx]
        mov dl,4
        xor eax,eax
        lea ebx ,[esi]
        es xlat
        

        push    dword 0      
        call    [exit]       