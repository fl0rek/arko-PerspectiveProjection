bits 64

section .text

global a_normalizeTo3d
a_normalizeTo3d:
        ; *matrix - RDI
        ; RDI - size [ignored]

        movups  xmm0, [rdi]
        pshufd  xmm1, xmm0, 0
        ;movshdup xmm1, xmm0
        ;movddup  xmm1, xmm1
        divps   xmm0, xmm1
        movups  [rdi], xmm0
        ret


        mov     [rdi+0],  dword __float32__(1.0)
        mov     [rdi+4],  dword __float32__(0.0)
        mov     [rdi+8],  dword __float32__(0.0)
        mov     [rdi+12], dword __float32__(0.0)
