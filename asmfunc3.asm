;SIMD YMM

section .text
bits 64
default rel
global vec_SIMDY

vec_SIMDY:
    ;to access d from shadow space
    mov r10, [rsp+40] 

    mov rax, rcx           
    shr rcx, 3             
    and rax, 7

    cmp rcx, 0
    je done_L1

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

done_L1:
    cmp rax, 0
    je tapos

    mov rcx, rax

remainder:
    vmaskmovps ymm1, ymm7, [r8]
    vmaskmovps ymm2, ymm7, [r9]
    vmaskmovps ymm3, ymm7, [r10]

    vmulps  ymm4, ymm2, ymm3
    vaddps  ymm0, ymm4, ymm1
    vmaskmovps [rdx], ymm7, ymm0

    add r8, 4
    add r9, 4 
    add r10, 4
    add rdx, 4
    loop remainder

tapos:
    ret
