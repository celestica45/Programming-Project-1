#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

//for assembly x86-64
extern void vec_x86(size_t n, float* a_x86, float* b, float* c, float* d);

//for SIMD XMM
extern void vec_SIMDX(size_t n, float* a_SIMDX, float* b, float* c, float* d);

//for SIMD YMM
extern void vec_SIMDY(size_t n, float* a_SIMDY, float* b, float* c, float* d);

// compute for the answer key
void vec_C(size_t n, float* a, float* b, float* c, float* d) {
	int i;
	for (i = 0; i < n; i++) {
		a[i] = b[i] + c[i] * d[i];
	}
}

int main() {
	const size_t ARRAY_SIZE = 1 << 20; // 2^20 = 1048576 ; 2^26 = 67108864; 2^28 = 268435456; 2^29= 536870912; 2^30=1073741824
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(float);
	int i;
	printf("\nNumber of elements = %zd\n\n", ARRAY_SIZE);
	printf("---------------------------------------------------\n");
	// timer variables
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
	for (i = 0; i < ARRAY_SIZE; i++) {
		b[i] = 1.0f;         // all ones
		c[i] = i % ARRAY_SIZE; //array a will contain values from 1 up to ARRAY_SIZE.
		d[i] = 1.0f;             // all ones
	}

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

	// array where Assembly will place the answer
	float* a_x86;
	a_x86 = (float*)malloc(ARRAY_BYTES);

	//zero-out the output
	for (i = 0; i < ARRAY_SIZE; i++) {
		a_x86[i] = 0.0f;
	}

	double total_time_x86 = 0.0; 

	for (int m = 0; m < 30; m++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_x86(ARRAY_SIZE, a_x86, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_x86 += elapse;

	}
	printf("\n---------------------------------------------------\n");
	printf("\nAverage Time in x86 (30 runs) = %f ms\n\n", total_time_x86 / 30.0);

	// error checking
	int fail = 0;
	for (i = 0; i < ARRAY_SIZE; i++) {
		if (a[i] != a_x86[i]) {
			fail += 1;
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
	a_SIMDX = (float*)malloc(ARRAY_BYTES); 

	//zero-out the array where SIMDX will put the output
	for (i = 0; i < ARRAY_SIZE; i++) {
		a_SIMDX[i] = 0.0f;
	}

	double total_time_SIMDX = 0.0;
	for (int g = 0; g < 30; g++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_SIMDX(ARRAY_SIZE, a_SIMDX, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_SIMDX += elapse;

	}
	printf("\n---------------------------------------------------\n");
	printf("\nAverage Time in SIMD XMM (30 runs) = %f ms\n\n", total_time_SIMDX / 30.0);


	// check if the array result is correct(error checking)
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

	//zero-out the array where SIMDY will put the output
	for (i = 0; i < ARRAY_SIZE; i++) {
		a_SIMDY[i] = 0.0f;
	}

	double total_time_SIMDY = 0.0;
	for (int f = 0; f < 30; f++) {
		QueryPerformanceCounter(&li); //start timer	
		start = li.QuadPart;
		vec_SIMDY(ARRAY_SIZE, a_SIMDY, b, c, d);
		QueryPerformanceCounter(&li); //end timer
		end = li.QuadPart;
		elapse = ((double)(end - start)) * 1000.0 / PCFreq;	// in milliseconds
		total_time_SIMDY += elapse;

	}
	printf("\n---------------------------------------------------\n");
	printf("\nAverage Time in SIMD YMM (30 runs) = %f ms\n\n", total_time_SIMDY / 30.0);

	// check if the array result is correct(error checking)
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

	printf("\n---------------------------------------------------");

	//free the memory
	free(a);
	free(b);
	free(c);
	free(d);
	free(a_x86);
	free(a_SIMDX);
	free(a_SIMDY);
	return 0;
}