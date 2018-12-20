%include "io.inc"
%include "C:\Users\kukumbers\Documents\Programming\UrFU\Computer Architecture\lab3\myMacros.mac"
%define size 64

section .bss
    input resb size
    output resb size
    char resb 1

section .text
global CMAIN
CMAIN:
    GET_STRING input, size
    GET_CHAR char
    mov ESI, input
    MOV EDI, output
    
    function: myMacros char, output
    
    xor eax, eax
    ret