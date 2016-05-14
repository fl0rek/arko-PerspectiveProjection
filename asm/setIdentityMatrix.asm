bits 64

section .text

global a_setIdentityMatrix
a_setIdentityMatrix:
        ; *matrix - RDI
        ; RDI - size [ignored]

        mov     [rdi+0],  dword __float32__(1.0)
        mov     [rdi+4],  dword __float32__(0.0)
        mov     [rdi+8],  dword __float32__(0.0)
        mov     [rdi+12], dword __float32__(0.0)
        mov     [rdi+16], dword __float32__(0.0)
        mov     [rdi+20], dword __float32__(1.0)
        mov     [rdi+24], dword __float32__(0.0)
        mov     [rdi+28], dword __float32__(0.0)
        mov     [rdi+32], dword __float32__(0.0)
        mov     [rdi+36], dword __float32__(0.0)
        mov     [rdi+40], dword __float32__(1.0)
        mov     [rdi+44], dword __float32__(0.0)
        mov     [rdi+48], dword __float32__(0.0)
        mov     [rdi+52], dword __float32__(0.0)
        mov     [rdi+56], dword __float32__(0.0)
        mov     [rdi+60], dword __float32__(1.0)
        ret
