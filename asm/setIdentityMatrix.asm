bits 64

section .text

global a_setIdentityMatrix
a_setIdentityMatrix:
        ; *matrix - RDI
        ; RDI - size [ignored]

        mov     rsi, 0
        mov     [rax+0], rsi
        mov     [rax+1], rsi
        ret
