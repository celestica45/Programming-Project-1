;SIMD XMM

section .text
bits 64
default rel
global vec_SIMDX

vec_SIMDX:
    shr rcx, 2
    mov r10, [rsp+40]

L1:
    vmovdqu xmm1, [r8]
    vmovdqu xmm2, [r9]
    vmovdqu xmm3, [r10]

    vmulps xmm4, xmm2, xmm3 
    vaddps xmm0, xmm4, xmm1 
    vmovdqu [rdx], xmm0     

    add r8, 16
    add r9, 16
    add r10, 16
    add rdx, 16
    loop L1             

    ret