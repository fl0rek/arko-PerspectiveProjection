bits 64

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
        movups  [rdi], xmm0
        ret
