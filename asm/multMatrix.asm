bits 64

section .text

global a_multMatrix
a_multMatrix:
        ; rdi - lhs transposed matrix
        ; rsi - rhs transposed matrix
        ;

        movups          xmm8,  [rdi]
        movups          xmm9,  [rdi+16]
        movups          xmm10, [rdi+32]
        movups          xmm11, [rdi+48]


        movaps          xmm12, xmm8
        movaps          xmm13, xmm9
        movaps          xmm14, xmm10
        movaps          xmm15, xmm11
        
;extern a_transpose_loaded
;        call            a_transpose_loaded
        ;lhs in xmm12-15

        movups          xmm4, [rsi]
        movups          xmm5, [rsi+16]
        movups          xmm6, [rsi+32]
        movups          xmm7, [rsi+48]

extern a_multVector_loaded
        movaps          xmm0, xmm12
        call            a_multVector_loaded
        movaps          xmm12, xmm2

        movaps          xmm0, xmm13
        call            a_multVector_loaded
        movaps          xmm13, xmm2

        movaps          xmm0, xmm14
        call            a_multVector_loaded
        movaps          xmm14, xmm2

        movaps          xmm0, xmm15
        call            a_multVector_loaded
        movaps          xmm15, xmm2

        movups          [rdi], xmm12
        movups          [rdi+16], xmm13
        movups          [rdi+32], xmm14
        movups          [rdi+48], xmm15

global foo1
foo1:
        ret
