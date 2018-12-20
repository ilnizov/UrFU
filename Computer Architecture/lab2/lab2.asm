%include "io.inc"
%define size 128

section .bss
    input resb size
    output resb size * 2
    char resb 1

section .text
global CMAIN
CMAIN:
    GET_STRING input, size
    GET_CHAR char
    mov ESI, input
    mov EDI, output
    
    .checkString:
        call .loadByte
        cmp AL, 0
        je .printOutput
        cmp AL, byte[char]
        jne .checkString
        call .findNeighbors
        jmp .checkString
        
    .printOutput:
        PRINT_STRING output
        ret
    
    .loadByte:
        mov AL, byte[ESI]
        inc ESI
        ret
    
    .saveByte:
        mov byte[EDI], AL
        inc EDI
        ret
        
    .findNeighbors:
        times 2 push ESI
        call .getLeft
        pop ESI
        call .getRight
        pop ESI
        ret
        
    .getLeft:
        times 2 dec ESI
        call .loadByte
        cmp AL, 0
        jne .end
        call .getLast
        
    .end:
        call .saveByte
        ret
        
    .getRight:
        call .loadByte
        cmp AL, 0x0A
        je .getFirst
        cmp AL, 0
        je .getFirst
        jmp .end
        
    .getFirst:
        mov ESI, input
        call .loadByte
        ret
    
    .getLast:
        call .loadByte
        cmp byte[ESI], 0x0A
        je .end
        jmp .getLast
        
    xor eax, eax
    ret