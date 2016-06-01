bits 64

section .data
extern projectionMatrixOriginal

section .bss
align 16
a_projMatrix:     resb 64
a_tempMatrix:     resb 64
a_angleX:         resb 4
a_angleY:         resb 4

;a_projectedVertices: resb 64
a_projectedVertices: resb 256

section .text
global a_redraw
a_redraw:
        ; rdi - p
        ; xmm0 - angleX
        ; xmm1 - angleY

        push    rbp
        push    rbx

        movups  [a_angleX], xmm0
        movups  [a_angleY], xmm1
        mov     r12, rdi

        movups  xmm0, [projectionMatrixOriginal]
        movups  xmm1, [projectionMatrixOriginal+16]
        movups  xmm2, [projectionMatrixOriginal+32]
        movups  xmm3, [projectionMatrixOriginal+48]

        movaps  [a_projMatrix], xmm0
        movaps  [a_projMatrix+16], xmm1
        movaps  [a_projMatrix+32], xmm2
        movaps  [a_projMatrix+48], xmm3

dddd:
extern a_doTranslationMatrix
extern a_multMatrix
extern a_doRotationMatrixX
extern a_doRotationMatrixY
extern a_transpose

        mov     rdi, a_tempMatrix
        xor     eax, eax
        mov     ebx, dword __float32__(-3.0) ; neither do I
        movd    xmm0, eax
        movd    xmm1, eax
        movd    xmm2, ebx
        call a_doTranslationMatrix
cccc:
        mov     rdi, a_projMatrix
        mov     rsi, a_tempMatrix
        call    a_multMatrix

        mov     rdi, a_tempMatrix
        movq     xmm0, [a_angleX]
        call    a_doRotationMatrixX

        mov     rdi, a_projMatrix
        mov     rsi, a_tempMatrix
        call    a_multMatrix

        mov     rdi, a_tempMatrix
        movq     xmm0, [a_angleY]
        call    a_doRotationMatrixY

        mov     rdi, a_projMatrix
        mov     rsi, a_tempMatrix
        call    a_multMatrix

        call    a_transpose
mat_done:

extern vertices
extern vertice_num
        mov     rbx, [vertice_num]
        add     rbx, rbx

        cmp     rbx, 0
        jz      vertices_projection_done

        movups  xmm4, [a_projMatrix]
        movups  xmm5, [a_projMatrix+16]
        movups  xmm6, [a_projMatrix+32]
        movups  xmm7, [a_projMatrix+48]
vertices_projection:
        dec     rbx
        dec     rbx

        lea     rsi, [vertices + rbx*8]
        lea     rdi, [a_projectedVertices + rbx*8]


        movups  xmm0, [rsi]
extern a_multVector_loaded
extern a_normalizeTo3d_loaded
        call    a_multVector_loaded
        movaps  xmm0, xmm2
        call    a_normalizeTo3d_loaded
        ;movups  [rdi], xmm0
global noz
noz:
        cmp     rbx, 0
        jnz      vertices_projection
vertices_projection_done:


extern lines
extern line_num

        mov     rbx, [line_num]

        cmp     rbx, 0
        jz      lines_draw_done
lines_draw:
        dec     rbx

        lea     rdi, [lines+rbx*8]

        ;;what the actual fuck
        xor     rax, rax
        mov     ax, [rdi]
        mov     r8, rax
        mov     ax, [rdi+4]
        mov     r9, rax

        shl     r8, 4
        shl     r9, 4

        ;add     r8, a_projectedVertices
        ;add     r9, a_projectedVertices

        mov     rdi, r12
dd:     ; p seems to be sane

        ;mov     rsi, r8
        ;mov     rdx, r9
        movss   xmm0, [r8+a_projectedVertices]
        movss   xmm1, [r8+a_projectedVertices+4]
        movss   xmm2, [r9+a_projectedVertices]
        movss   xmm3, [r9+a_projectedVertices+4]
;
extern a_drawLineI
extern drawLineI
        call    a_drawLineI

        ;cmp     rbx, 0
        test    rbx, rbx
        jnz     lines_draw
lines_draw_done:


        pop     rbx
        pop     rbp
