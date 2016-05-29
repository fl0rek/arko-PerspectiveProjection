bits 64

section .text

global a_multVector
global a_multVector_loaded
a_multVector:
        ; rdi - v
        ; rsi - m column major

        movups          xmm4, [rsi]
        movups          xmm5, [rsi+16]
        movups          xmm6, [rsi+32]
        movups          xmm7, [rsi+48]

        movups          xmm0, [rdi]
loaded:
a_multVector_loaded:
        xorps           xmm2, xmm2

        movups          xmm1, xmm0
        shufps          xmm1, xmm1, 0x00
        mulps           xmm1, xmm4
        addps           xmm2, xmm1

        movups          xmm1, xmm0
        shufps          xmm1, xmm1, 0x55
        mulps           xmm1, xmm5
        addps           xmm2, xmm1

        movups          xmm1, xmm0
        shufps          xmm1, xmm1, 0xAA
        mulps           xmm1, xmm6
        addps           xmm2, xmm1

        movups          xmm1, xmm0
        shufps          xmm1, xmm1, 0xFF
        mulps           xmm1, xmm7
        addps           xmm2, xmm1

        movups          [rdi], xmm2

        ret

;%macro transposition 1
;        mov     xmm0, %1
;        punpckldq
