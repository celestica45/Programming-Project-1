# SIMD Programming Project
Members: Guillermo, Ty, Ughoc

CSC612M G03

## Introduction
This project explores different ways to efficiently calculate the operation <b>A[i] = B[i] + C[i] × D[i]</b> for large arrays of floating-point numbers. Four kernels are implemented:

- **Standard C code**
- **x86-64 assembly (non-SIMD)**
- **SIMD AVX2 assembly using XMM registers**
- **SIMD AVX2 assembly using YMM registers**

All kernels were benchmarked in both **DEBUG** and **RELEASE** build modes to compare performance.

For each method, we:
- Initialize vectors with known values
- Measure execution time over large input sizes and multiple runs
- Check that results match the C reference output
- Demonstrate how each version deals with vector sizes that aren't a perfect fit for SIMD instructions

All source code, project files, timing results, screenshots, and correctness checks are included in this repository. From these results, we compare performance and discuss which implementation works best and why.

## Program Overview
* Write the kernel in (1) C program; (2) an x86-64 assembly language; (3) x86-64 SIMD AVX2 assembly language using XMM register; (4) x86-64 SIMD AVX2 assembly language using YMM register. The kernel is to perform vector triad for vectors B C, D and place the result in vector A. For each kernel version, time the process for vector size n = {2^(20), 2^(26), and 2^(30)}

* Input: Scalar variable n (integer) contains the length of the vector; Vectors B, C and D are single-precision float.
    * ***Input Initialization***: Arrays B, C, and D were initialized using trigonometric functions (sin, cos, tan) as shown below:
    
       ![Alt text](screenshots/initialization.png)
* Process: A[i] = B[i] + C [i] * D[i]

* Output: store the result in vector A. Display the first 5 and the last 5 elements of vector A for verification.

## Program output with Execution Time and Correctness Check
### Debug Mode
* 2^20

   ![Alt text](screenshots/20/20D.png)

  
* 2^26

   ![Alt text](screenshots/26/26D.png)

  
* 2^30

   ![Alt text](screenshots/30/30D.png)

### Release Mode
* 2^20

   ![Alt text](screenshots/20/20R.png)

  
* 2^26

   ![Alt text](screenshots/26/26R.png)

  
* 2^30

   ![Alt text](screenshots/30/30R.png)

## Boundary Check
### XMM

   <img width="313" height="187" alt="image" src="https://github.com/user-attachments/assets/7ec0fb03-1b5e-4595-8d39-e561446c834a" />

   `rax` takes in `rcx`, which contains the total amount of elements in the array. `shr rcx, 2` divides the array into groups of 4. The boundary checking comes from `and rax, 3`, which gets the remainder of the array size divided by 4 (rax % 4) so that the program can compute for the remaining items later on.

  <img width="279" height="250" alt="image" src="https://github.com/user-attachments/assets/3455733e-9cd9-45b2-8d72-1e74e4751ab4" />

   This part of the program calculates for the result one element at a time instead of doing it 4 at a time, looping for a maximum of 3 times. 

   Sample:
   * Array Size = 4099 (4099 / 4 = 1024 remainder 3)

   <img width="351" height="982" alt="boundary 1" src="https://github.com/user-attachments/assets/6c9b2ead-57f7-409d-9a36-c415e239fabf" />

### YMM
   A similar process can be done for YMM.
      
   <img width="295" height="184" alt="image" src="https://github.com/user-attachments/assets/cb06dba0-cd63-41dc-89ef-61a4f9102116" />

   `rax` takes in `rcx`, which contains the total amount of elements in the array. `shr rcx, 3` divides the array into groups of 8. The boundary checking comes from `and rax, 7`, which gets the remainder of the array size divided by 8 (rax % 8) so that the program can compute for the remaining items later on.
   
   <img width="247" height="262" alt="image" src="https://github.com/user-attachments/assets/2dcd347c-0184-4fcd-b59a-36d461b86d08" />

  This part of the program calculates for the result one element at a time instead of doing it 8 at a time, looping for a maximum of 7 times. 

   Sample:
   * Array Size = 24007 (24007 / 8 = 3000 remainder 7)
     
     <img width="342" height="987" alt="image" src="https://github.com/user-attachments/assets/68d1a248-4acd-47cc-9b7c-5973ca267f1c" />

## Summary Table of Execution Time and Comparative Result
### Debug

   ![Alt text](screenshots/summary_debug.png)

### Release

   ![Alt text](screenshots/summary_release.png)

### SIMD Speed-up(XMM vs YMM)
   * Debug
     
      ![Alt text](screenshots/simd_debug.png)
   * Release
     
      ![Alt text](screenshots/simd_release.png)


## Analysis of Results

### Overall Performance Trend

   From the results, we observe a clear performance hierarchy in the Debug build. In Debug mode, SIMD provided up to 4.4× speedup over the baseline C implementation, showing the strong advantage of vectorization when compiler optimizations are disabled. In contrast, in Release mode, all implementations converge to nearly the same performance, with differences shrinking to within ±5%. This confirms that the Release compiler automatically vectorized and optimized the C version, minimizing the manual SIMD advantage.

### Low Arithmetic Intensity

  The kernel performs only two floating-point operations per iteration (one multiplication and one addition) but transfers 16 bytes of memory, with 4 bytes per element. There are 3 loads (B[i], C[i], and D[i]), with 1 store (A[i]) with 4 bytes each, which leads to 16 bytes.

   That yields a low arithmetic intensity of:

   $`\text{Arithmetic Intensity} = \frac{2 \text{ flops}}{16 \text{ bytes}} = 0.125 \text{ flop/byte}`$

   Since the arithmetic intensity is low, it means that performance is limited by memory bandwidth, not by ALU throughput. Once arrays exceed cache capacity (such as when 2 is raised to 26 or 30), data must be fetched from DRAM. Even if the CPU executes wider SIMD operations, the bottleneck shifts to how fast data can be supplied to registers. Thus, YMM cannot be 2× faster than XMM, since both are constrained by the same DRAM bandwidth. 

### YMM and XMM 

   Although YMM registers are 256 bits wide (processing 8 floats) versus XMM’s 128 bits (4 floats), the observed speedup is only around 1.08× in practice. Several factors explain this:

   1. Memory Bandwidth Saturation. Doubling the vector width does not double the available bandwidth. The memory subsystem already operates near its limit, so YMM gains little in sustained throughput.
   2. Same Memory Bottleneck. Once memory throughput is saturated, instruction-level parallelism or SIMD width no longer impacts total runtime.

   With that being said, even though YMM executes more elements per instruction, the bottleneck isn’t the compute pipeline. Rather, it is the data movement, resulting in only a slight speedup.

### Debug vs. Release Behavior

   In Debug mode, there is a large difference between C and SIMD implementations. Debug builds disable optimizations such as loop unrolling, inlining, and auto-vectorization. This forces the C compiler to use simple, scalar operations, while SIMD assembly manually leverages hardware vector units, which does explain the 4× gain in Debug mode.

   In Release mode, however, the compiler automatically vectorizes and optimizes the C code using SSE or AVX instructions internally. As a result, manually written SIMD code provides no major advantage, and all four implementations achieve similar performance. The small performance differences are dominated by memory latency variations and cache behavior, not by arithmetic throughput.


### Vector Size

When comparing different array sizes, the smallest size of 2^20 fits in the cache. SIMD achieves its best relative speed because the working set resides mostly in L2/L3 cache, allowing the CPU to exploit full vector throughput. 

At 2^26 and 2^30, execution times scale almost linearly with array size, indicating that performance is dominated by memory access, not computation.

Looking at these results, it suggests that beyond a certain point, increasing SIMD width offers diminishing returns due to saturated memory bandwidth.

This behavior closely matches the Roofline performance model, where the compute-bound region (small data) transitions to a memory-bound region (large data).

### Boundary Handling

Boundary handling was correctly implemented for cases where the number of elements isn’t divisible by the SIMD width (4 for XMM, 8 for YMM).

For large arrays like 2^30, the remainder cost is negligible (<0.01%), but the correctness check confirms that both SIMD implementations handle the remainder elements properly.

### Key Insights

The vector triad is memory-bound, meaning that beyond a certain point, performance is determined by memory throughput, not CPU compute power.

SIMD improves efficiency by processing multiple elements per instruction, but its advantage caps out when data movement is dominating.

For workloads with higher arithmetic intensity (e.g., matrix multiplication or Fast Fourier Transform), YMM would most likely show a much larger speedup.


## References

Improving performance with SIMD intrinsics in three use cases - Stack Overflow. (2020, July 8). https://stackoverflow.blog/2020/07/08/improving-performance-with-simd-intrinsics-in-three-use-cases

Jelínek, J. (2023, December 8). Vectorization optimization in GCC | Red Hat Developer. Red Hat Developer. https://developers.redhat.com/articles/2023/12/08/vectorization-optimization-gcc

Nersc. (n.d.). Roofline Performance Model - NERSC documentation. https://docs.nersc.gov/tools/performance/roofline/

Roofline Model - an overview | ScienceDirect Topics. (n.d.). https://www.sciencedirect.com/topics/computer-science/roofline-model

What is a Frontier Model? (2025, April 6). Iguazio. https://www.iguazio.com/glossary/arithmetic-intensity/

