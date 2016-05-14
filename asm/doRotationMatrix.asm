bits 64

section .text

global a_doRotationMatrixY
global a_doRotationMatrixX

extern a_setIdentityMatrix

a_doRotationMatrixY:
a_doRotationMatrixX:
        ; *matrix - RDI
        call a_setIdentityMatrix
