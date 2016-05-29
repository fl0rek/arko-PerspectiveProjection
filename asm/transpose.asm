bits 64

global a_transpose
global a_transpose_loaded
a_transpose:
        movups          xmm8,  [rdi]
        movups          xmm9,  [rdi+16]
        movups          xmm10, [rdi+32]
        movups          xmm11, [rdi+48]

a_transpose_loaded:
        ;movaps          xmm0, xmm8
        ;punpckldq       xmm0, xmm9
        ;movaps          xmm1, xmm10
        ;punpckldq       xmm1, xmm11

        ;punpckhdq       xmm8, xmm9
        ;punpckhdq       xmm10, xmm11
        vpunpckldq      xmm0, xmm8, xmm9
        vpunpckldq      xmm1, xmm10, xmm11

        vpunpckhdq      xmm2, xmm8, xmm9
        vpunpckhdq      xmm3, xmm10, xmm11

global foo;
foo:
        ;movaps          xmm12, xmm0
        ;punpckldq       xmm12, xmm1
        ;movaps          xmm13, xmm0
        ;punpckhdq       xmm13, xmm1

        ;movaps          xmm14, xmm8
        ;punpckldq       xmm14, xmm10
        ;movaps          xmm15, xmm8
        ;punpckhdq       xmm15, xmm10

        vpunpcklqdq     xmm12, xmm0, xmm1
        vpunpckhqdq      xmm13, xmm0, xmm1
        vpunpcklqdq      xmm14, xmm2, xmm3
        vpunpckhqdq      xmm15, xmm2, xmm3

global goo
goo:

epilogue:
        movups          [rdi]   , xmm12
        movups          [rdi+16], xmm13
        movups          [rdi+32], xmm14
        movups          [rdi+48], xmm15

        ret
