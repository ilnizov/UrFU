%include "io.inc"
%define size 32

section .bss
    input resb size
    output resb size

section .text
global CMAIN
CMAIN:
    GET_STRING input, size
    mov ESI, input
    mov EDI, output
    cld
   
    .loop:
        lodsb
        cmp AL, 0
        je .exit
        cmp AL, " "
        je .loop
        stosb
        jmp .loop
        
    .exit:
        PRINT_STRING output
        
    xor eax, eax
    ret