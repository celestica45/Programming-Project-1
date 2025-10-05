#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>

//for assembly x86-64
extern void vec_x86(size_t n, float* a_x86, float* b, float* c, float* d);

//for SIMD XMM
extern void vec_SIMDX(size_t n, float* a_SIMDX, float* b, float* c, float* d);

//for SIMD YMM
extern void vec_SIMDY(size_t n, float* a_SIMDY, float* b, float* c, float* d);


void vec_C(size_t n, float* a, float* b, float* c, float* d) {
	int i;
	for (i = 0; i < n; i++) {
		a[i] = b[i] + c[i] * d[i]; // this is our answer key
	}
}

int main() {
	const size_t ARRAY_SIZE = 9; // 2^20 = 1048576
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
	int i;
	printf("Number of elements = %zd\n\n", ARRAY_SIZE);

	// timer variable
	LARGE_INTEGER li;				// use for timer
	long long int start, end;		// use for timer
	double PCFreq, elapse, elapse1; // use for timer
	QueryPerformanceFrequency(&li); //check if performance frequency is available
	PCFreq = (double)(li.QuadPart);

	//declare array (use malloc to declare very large data)
	float* a, * b, * c, * d;
	a = (float*)malloc(ARRAY_BYTES);
	b = (float*)malloc(ARRAY_BYTES);
	c = (float*)malloc(ARRAY_BYTES);
	d = (float*)malloc(ARRAY_BYTES);

	//initialize array

	//option1
	for (i = 0; i < ARRAY_SIZE; i++) {
		b[i] = 1.0f;         // all ones
		c[i] = i % ARRAY_SIZE;
		d[i] = 1.0f;             // all ones
	}

	//option2
	/*srand(time(NULL)); // seed once

	for (i = 0; i < ARRAY_SIZE; i++) {
		b[i] = (float)rand() / RAND_MAX;
		c[i] = (float)rand() / RAND_MAX;
		d[i] = (float)rand() / RAND_MAX;
	}*/


	// --------- start of the program in C ---------
	double total_time_C = 0.0;
	for (int t = 0; t < 30; t++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_C(ARRAY_SIZE, a, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_C += elapse;
			
	}
	printf("\nAverage Time in C (30 runs) = %f ms\n\n", total_time_C / 30.0);

	 // print first 5 elements
	printf("First 5 elements of a:\n");
	for (i = 0; i < 5; i++) {
		printf("a[%d] = %0.1f\n", i, a[i]);
	}

	// print last 5 elements
	printf("\nLast 5 elements of a:\n");
	for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
		printf("a[%d] = %0.1f\n", i, a[i]);
	}

	// ---------end of the program in C---------


	//--------- start of the program in Assembly(x86-64) ---------
	// kumusta si Assembly in relation to C

	// array where Assembly will place the answer
	float* a_x86;
	a_x86 = (float*)malloc(ARRAY_BYTES); // assembly output
	//zero-out the output
	for (i = 0; i < ARRAY_SIZE; i++) {
		a_x86[i] = 0.0f;
	}


	
	double total_time_x86 = 0.0;
	for (int t = 0; t < 30; t++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_x86(ARRAY_SIZE, a_x86, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_x86 += elapse;

	}
	printf("\nAverage Time in x86 (30 runs) = %f ms\n\n", total_time_x86 / 30.0);

	// check if the array result is correct(error checking)
	// note that this method of using 1 variable lang is the lazy method; do not do this in the MP and in real life
	// if there was no variable ans, you would have to declare another array and call it float j; im going to place answer in  j then compare j[i] to the answer key z[i]
	//float ANS = 3.0f;
	int fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (a[i] != a_x86[i]) {
			fail += 1; // fail
		}
	}
	if (fail > 0) {
		printf("Vec(x86-64): FAILED with %d error\n\n", fail);
	}
	else {
		printf("Vec(x86-64): PASS\n\n");
	}

	// print first 5 elements
	printf("First 5 elements of a_x86:\n");
	for (i = 0; i < 5; i++) {
		printf("a_86[%d] = %0.1f\n", i, a_x86[i]);
	}

	// print last 5 elements
	printf("\nLast 5 elements of a_x86:\n");
	for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
		printf("a_x86[%d] = %0.1f\n", i, a_x86[i]);
	}
	//--------- end of the program in Assembly(x86-64) ---------



	//--------- start of the program in SIMD XMM ---------


	// array where SIMD XMM will place the answer
	float* a_SIMDX;
	a_SIMDX = (float*)malloc(ARRAY_BYTES); // assembly output
	//zero-out the output
	for (i = 0; i < ARRAY_SIZE; i++) {
		a_SIMDX[i] = 0.0f;
	}

	double total_time_SIMDX = 0.0;
	for (int t = 0; t < 30; t++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_SIMDX(ARRAY_SIZE, a_SIMDX, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_SIMDX += elapse;

	}
	printf("\nAverage Time in SIMD XMM (30 runs) = %f ms\n\n", total_time_SIMDX / 30.0);


	// check if the array result is correct(error checking)
	// note that this method of using 1 variable lang is the lazy method; do not do this in the MP and in real life
	// if there was no variable ans, you would have to declare another array and call it float j; im going to place answer in  j then compare j[i] to the answer key z[i]
	//float ANS = 3.0f;
	fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (a[i] != a_SIMDX[i]) {
			fail += 1; // fail
		}
	}
	if (fail > 0) {
		printf("Vec(SIMD XMM): FAILED with %d error\n\n", fail);
	}
	else {
		printf("Vec(SIMD XMM): PASS\n\n");
	}

	// print first 5 elements
	printf("First 5 elements of a_SIMDX:\n");
	for (i = 0; i < 5; i++) {
		printf("a_SIMDX[%d] = %0.1f\n", i, a_SIMDX[i]);
	}

	// print last 5 elements
	printf("\nLast 5 elements of a_SIMDX:\n");
	for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
		printf("a_SIMDX[%d] = %0.1f\n", i, a_SIMDX[i]);
	}
	//--------- end of the program in Assembly(x86-64) ---------





	//--------- start of the program in SIMD YMM ---------

	// array where SIMD YMM will place the answer
	float* a_SIMDY;
	a_SIMDY = (float*)malloc(ARRAY_BYTES); // SIMD YMM output
	//zero-out the output
	for (i = 0; i < ARRAY_SIZE; i++) {
		a_SIMDY[i] = 0.0f;
	}

	double total_time_SIMDY = 0.0;
	for (int t = 0; t < 30; t++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_SIMDY(ARRAY_SIZE, a_SIMDY, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_SIMDY += elapse;

	}
	printf("\nAverage Time in SIMD YMM (30 runs) = %f ms\n\n", total_time_SIMDY / 30.0);


	// check if the array result is correct(error checking)
	// note that this method of using 1 variable lang is the lazy method; do not do this in the MP and in real life
	// if there was no variable ans, you would have to declare another array and call it float j; im going to place answer in  j then compare j[i] to the answer key z[i]
	//float ANS = 3.0f;
	fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (a[i] != a_SIMDY[i]) {
			fail += 1; // fail
		}
	}
	if (fail > 0) {
		printf("Vec(SIMD YMM): FAILED with %d error\n\n", fail);
	}
	else {
		printf("Vec(SIMD YMM): PASS\n\n");
	}

	// print first 5 elements
	printf("First 5 elements of a_SIMDY:\n");
	for (i = 0; i < 5; i++) {
		printf("a_SIMDY[%d] = %0.1f\n", i, a_SIMDY[i]);
	}

	// print last 5 elements
	printf("\nLast 5 elements of a_SIMDY:\n");
	for (i = ARRAY_SIZE - 5; i < ARRAY_SIZE; i++) {
		printf("a_SIMDY[%d] = %0.1f\n", i, a_SIMDY[i]);
	}
	//--------- end of the program in SIMD YMM ---------



	//free the memory
	free(a);
	free(b);
	free(c);
	free(d);
	return 0;
}