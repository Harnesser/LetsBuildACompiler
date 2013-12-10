# http://en.wikibooks.org/wiki/X86_Assembly/NASM_Syntax#Example_I.2FO_.28Linux_and_BSD.29
# http://www.dreamincode.net/forums/topic/286248-nasm-linux-terminal-inputoutput-wint-80h/

sys_exit        equ     1
sys_read        equ     3
sys_write       equ     4
stdin           equ     0
stdout          equ     1
stderr          equ     3

SECTION     .data
szName          db      "What is your name? "
Name_Len        equ     $-szName
szHello         db      "Hello ", 0
Hello_Len       equ     $-szHello

SECTION     .bss
lpBuffer        resb    7
Buf_Len         equ     $-lpBuffer
    
SECTION     .text
global      _start
    
_start:
    mov     ecx, szName
    mov     edx, Name_Len
    call    DisplayText

    mov     ecx, lpBuffer
    mov     edx, Buf_Len
    call    ReadText
    push    eax
    
    mov     ecx, szHello
    mov     edx, Hello_Len
    call    DisplayText
    
    pop     edx
    mov     ecx, lpBuffer
    call    DisplayText
    jmp     Exit
    
DisplayText:
    mov     eax, sys_write
    mov     ebx, stdout
    int     80H 
    ret
    
ReadText:
    mov     ebx, stdin
    mov     eax, sys_read
    int     80H
    ret

Exit:  
    mov     eax, sys_exit
    xor     ebx, ebx
    int     80H

