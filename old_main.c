#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

//for assembly x86-64
extern void vecadd_x86(size_t n, float* z, float* x, float* y);
						//rcx    rdx=addr   r8=addr   r9=addr 
	// n has to be the first one since it will be used as the counter for the loop
//for assembly SIMD XMM
extern void vecadd_SIMDX(size_t n, float* z, float* x, float* y);
						//rcx    rdx=addr   r8=addr   r9=addr 
//for assembly SIMD YMM
extern void vecadd_SIMDY(size_t n, float* z, float* x, float* y);
//rcx    rdx=addr   r8=addr   r9=addr 

void vecadd_C(size_t n, float* z, float* x, float* y) {
	int i;
	for (i = 0; i < n; i++) {
		z[i] = x[i] + y[i]; // this is our answer key
	}
}

int main() {
	const size_t ARRAY_SIZE = 1 << 24; // 2^24 = 167777216
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
	int i; 
	printf("Number of elements = %zd\n\n", ARRAY_SIZE);

	// timer variable
	LARGE_INTEGER li;				// use for timer
	long long int start, end;		// use for time
	double PCFreq, elapse, elapse1; // use for timer
	QueryPerformanceFrequency(&li); //check if performance frequency is available
	PCFreq = (double)(li.QuadPart);

	//declare array (use malloc to declare very large data)
	float* x, *y, *z;
	x = (float*)malloc(ARRAY_BYTES);
	y = (float*)malloc(ARRAY_BYTES);
	z = (float*)malloc(ARRAY_BYTES);

	//initialize array
	for (i = 0; i < ARRAY_SIZE; i++) {
		x[i] = 1.0f;
		y[i] = 2.0f;
	}	

	// --------- start of the program in C ---------
	QueryPerformanceCounter(&li); //start timer	
	start = li.QuadPart;
		vecadd_C(ARRAY_SIZE, z, x, y);
	QueryPerformanceCounter(&li); //end timer
	end = li.QuadPart;
	elapse = ((double)(end - start)) *1000.0 / PCFreq;	// in milliseconds
	printf("Time in C = %f ms\n  \n", elapse);
	// ---------end of the program in C---------

	//--------- start of the program in Assembly(x86-64) ---------
	// kumusta si Assembly in relation to C
	
	//zero-out the output
	for (i=0; i<ARRAY_SIZE; i++){
		z[i] = 0.0f;
	}
	QueryPerformanceCounter(&li); //start timer
	// in reality, we dont do this just 1 time
	// loop 30 times then get the average
	start = li.QuadPart;
		vecadd_x86(ARRAY_SIZE, z, x, y); //call the assembly function

	QueryPerformanceCounter(&li); //end timer
	end = li.QuadPart;
	elapse = ((double)(end - start)) * 1000.0 / PCFreq; // in 
	printf("Time in x86-64 assembly = %f ms\n", elapse);

	// check if the array result is correct(error checking)
	// note that this method of using 1 variable lang is the lazy method; do not do this in the MP and in real life
	// if there was no variable ans, you would have to declare another array and call it float j; im going to place answer in  j then compare j[i] to the answer key z[i]
	float ANS = 3.0f;
	int fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (z[i] != ANS) {
			fail += 1; // fail
		}
	}
	if (fail > 0) {
		printf("VecAdd(x86-64): FAILED with %d error\n\n", fail);
	}
	else {
		printf("VecAdd(x86-64): PASS\n\n");
	}
	//--------- end of the program in Assembly(x86-64) ---------




	//--------- start of the program in SIMD XMM register ---------
	/*zero out the output*/
	for (i = 0; i < ARRAY_SIZE; i++) {
		z[i] = 0.0f;
	}

	QueryPerformanceCounter(&li); //start timer
	// in reality, we dont do this just 1 time
	// loop 30 times then get the average
	start = li.QuadPart;
	vecadd_SIMDX(ARRAY_SIZE, z, x, y); //call the assembly function

	QueryPerformanceCounter(&li); //end timer
	end = li.QuadPart;
	elapse = ((double)(end - start)) * 1000.0 / PCFreq; // in 
	printf("Time in SIMD XMM = %f ms\n", elapse);

	// check if the array result is correct(error checking)
	// note that this method of using 1 variable lang is the lazy method; do not do this in the MP and in real life
	// if there was no variable ans, you would have to declare another array and call it float j; im going to place answer in  j then compare j[i] to the answer key z[i]
	ANS = 3.0f;
	fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (z[i] != ANS) {
			fail += 1; // fail
		}
	}
	if (fail > 0) {
		printf("VecAdd(SIMD XMM): FAILED with %d error\n\n", fail);
	}
	else {
		printf("VecAdd(SIMD XMM): PASS\n\n");
	}
	//--------- end of the program in SIMD XMM register ---------




	//--------- start of the program in SIMD YMM register ---------
	/*zero out the output*/
	for (i = 0; i < ARRAY_SIZE; i++) {
		z[i] = 0.0f;
	}

	QueryPerformanceCounter(&li); //start timer
	// in reality, we dont do this just 1 time
	// loop 30 times then get the average
	start = li.QuadPart;
	vecadd_SIMDY(ARRAY_SIZE, z, x, y); //call the assembly function

	QueryPerformanceCounter(&li); //end timer
	end = li.QuadPart;
	elapse = ((double)(end - start)) * 1000.0 / PCFreq; // in 
	printf("Time in SIMD YMM = %f ms\n", elapse);

	// check if the array result is correct(error checking)
	// note that this method of using 1 variable lang is the lazy method; do not do this in the MP and in real life
	// if there was no variable ans, you would have to declare another array and call it float j; im going to place answer in  j then compare j[i] to the answer key z[i]
	ANS = 3.0f;
	fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (z[i] != ANS) {
			fail += 1; // fail
		}
	}
	if (fail > 0) {
		printf("VecAdd(SIMD YMM): FAILED with %d error\n\n", fail);
	}
	else {
		printf("VecAdd(SIMD YMM): PASS\n\n");
	}
	//--------- end of the program in SIMD YMM register ---------

	//free the memory
	free(x);
	free(y);
	free(z);
	return 0;
}