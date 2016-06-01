bits 64

section .data
one: dd 1.0

section .text


global a_normalizeTo3d
a_normalizeTo3d:
        ; *matrix - RDI
        ; RsI - size [ignored]

        movups  xmm0, [rdi]
global a_normalizeTo3d_loaded
a_normalizeTo3d_loaded:
        pshufd  xmm1, xmm0, 0xFF
        ;movshdup xmm1, xmm0
        ;movddup  xmm1, xmm1
        divps   xmm0, xmm1
        movss   xmm1, [one]
        pshufd  xmm1, xmm1, 0x0
        addps   xmm0, xmm1
        addps   xmm1, xmm1
        divps   xmm0, xmm1
        movups  [rdi], xmm0
        ret
