# SIMD Programming Project
Members: Guillermo, Ty, Ughoc

CSC612M G03

## Project Overview
* Write the kernel in (1) C program; (2) an x86-64 assembly language; (3) x86-64 SIMD AVX2 assembly language using XMM register; (4) x86-64 SIMD AVX2 assembly language using YMM register. The kernel is to perform vector triad for vectors B C, D and place the result in vector A.

* Input: Scalar variable n (integer) contains the length of the vector; Vectors B, C and D are single-precision float.
    * ***Input Initialization***: Arrays B,C, and D were initialized using trigonometric functions (sin, cos, tan) as shown below:
    
       ![Alt text](screenshots/initialization.png)
* Process: A[i] = B[i] + C [i] * D[i]

* Output: store the result in vector A. Display the first 5 and the last 5 elements of vector A for verification.

## Results 
### Program output with Execution Time and Correctness Check
#### Debug Mode
* 2^20
    ![Alt text](screenshots/20/20D.png)
* 2^26
* 2^30
