bits 64

section .data

minus_zero: dd __float32__(-0.0)

section .text

global a_drawLineI

%macro a_putpixel 0
        mov     r11, rdi
        cvtss2si        r9,  xmm0
        cvtss2si        r10, xmm1

        imul     r10, r8
        imul     r10, 4
        imul     r9, 4

        add     r11, r9
        add     r11, r10

        mov     [r11+0], byte 0
        mov     [r11+1], byte 0
        mov     [r11+2], byte 0
%endmacro

a_drawLineI:
        ; rdi - p
        ; xmm0 -x0
        ; xmm1 -y0
        ; xmm2 -x1
        ; xmm3 -y1

        push    rbp
        mov     rbp, rsp

a_scale:
extern window_height
extern window_width
extern window_width_int

        xor     r8, r8
        mov     r8, [window_width_int]

        movss   xmm4, [window_width]
        movss   xmm5, [window_height]

        mulss   xmm0, xmm4
        mulss   xmm2, xmm4

        mulss   xmm1, xmm5
        mulss   xmm3, xmm5
a_scale_done:

a_drawLineXD:
        vsubss  xmm6, xmm2, xmm0
        vsubss  xmm7, xmm3, xmm1

        movss   xmm8, [minus_zero]
        movss   xmm9, [minus_zero]

        andnps  xmm8, xmm6
        andnps  xmm9, xmm7

        maxps   xmm8, xmm9

        divss   xmm6, xmm8
        divss   xmm7, xmm8
xd:
        cvtss2si        r13, xmm8

; =======================================
        mov     r11, rdi
        cvtss2si        r9,  xmm0
        cvtss2si        r10, xmm1
dds:
        imul     r10, r8
        imul     r10, 4
        imul     r9, 4

        add     r11, r9
        add     r11, r10

        mov     [r11+0], byte 0
        mov     [r11+1], byte 0
        mov     [r11+2], byte 0
; =======================================

a_drawLineXD_iteration:
        dec     r13
        test    r13, r13
        jz      a_drawLineXD_done

        addss   xmm0, xmm6
        addss   xmm1, xmm7

; =======================================
        mov     r11, rdi
        cvtss2si        r9,  xmm0
        cvtss2si        r10, xmm1
ddr:
        imul     r10, r8
        imul     r10, 4
        imul     r9, 4

        add     r11, r9
        add     r11, r10

        mov     [r11+0], byte 0
        mov     [r11+1], byte 0
        mov     [r11+2], byte 0
; =======================================

        jmp     a_drawLineXD_iteration

a_drawLineXD_done:

        mov     rsp, rbp
        pop     rbp
        ret
