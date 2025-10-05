;SIMD XMM

section .text
bits 64
default rel
global vec_SIMDX

vec_SIMDX:
    ;to access d from shadow space
    mov r10, [rsp+40]

    mov rax, rcx
    shr rcx, 2
    and rax, 3
    
    cmp rcx, 0
    je done_L1

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

done_L1:
    ; remainder loop
    cmp rax, 0
    je tapos

    mov rcx, rax

remainder:
    vmovss xmm1, [r8]
    vmovss xmm2, [r9]
    vmovss xmm3, [r10]

    vmulss xmm4, xmm2, xmm3 
    vaddss xmm0, xmm4, xmm1 
    vmovss [rdx], xmm0  

    add r8, 4
    add r9, 4
    add r10, 4
    add rdx, 4
    loop remainder

tapos:
    ret