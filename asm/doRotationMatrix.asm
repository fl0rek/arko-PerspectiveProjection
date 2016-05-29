bits 64

section .text

global a_doRotationMatrixY
global a_doRotationMatrixX

extern a_setIdentityMatrix

a_doRotationMatrixY:

        ; *matrix - RDI
        ; xmm0 - rot
        push rbp
        mov rbp, rsp

        call a_setIdentityMatrix

        sub     rsp, 8
        movd    [rsp], xmm0

        fld     dword [rsp]
        fsincos
        fst     dword [rdi+ 0]
        fstp    dword [rdi+40]

        fst     dword [rdi+ 8]
        fldz
        fsubrp
        fstp    dword [rdi+32]

        jmp  epilogue
a_doRotationMatrixX:
        ; *matrix - RDI
        ; xmm0 - rot
        push rbp
        mov rbp, rsp

        call a_setIdentityMatrix

        sub     rsp, 8
        movd    [rsp], xmm0

        fld     dword [rsp]
        fsincos
        fst     dword [rdi+20]
        fstp    dword [rdi+40]

        fst     dword [rdi+36]
        fldz
        fsubrp
        fstp    dword [rdi+24]




        ;fld xmm0
epilogue:
        mov rsp, rbp
        pop rbp
        ret
