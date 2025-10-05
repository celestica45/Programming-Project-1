;Assembly x86-64

section .text
bits 64
default rel
global vec_x86

vec_x86:
    mov r10, [rsp+40]

L1:
    vmovss xmm1, [r8]
    vmovss xmm2, [r9]
    vmovss xmm3, [r10]

    vmulss xmm4, xmm2, xmm3 ;c[i] * d[i]
    vaddss xmm0, xmm4, xmm1 ;b[i] + c[i]*d[i]
    vmovss [rdx], xmm0

    add r8, 4
    add r9, 4
    add r10, 4
    add rdx, 4
    loop L1             

    ret