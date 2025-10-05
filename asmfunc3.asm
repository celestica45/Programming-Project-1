;SIMD YMM

section .text
bits 64
default rel
global vec_SIMDY

vec_SIMDY:
    shr rcx, 3
    mov r10, [rsp+40]

L1:
    vmovdqu ymm1, [r8]
    vmovdqu ymm2, [r9]
    vmovdqu ymm3, [r10]

    vmulps ymm4, ymm2, ymm3 
    vaddps ymm0, ymm4, ymm1 
    vmovdqu [rdx], ymm0     

    add r8, 32
    add r9, 32
    add r10, 32
    add rdx, 32
    loop L1             

    ret