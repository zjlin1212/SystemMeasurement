#include "CPU.h"
#include <iostream>

using namespace std;

static inline uint64_t rdtsc(void) {
	uint32_t lo, hi;
	__asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
	return (((uint64_t)hi << 32) | lo);
}


void CPU::readOverhead(){
	double sum = 0;
	uint64_t start, end;
	rdtsc();
	for (int i = 0; i < MAXITER; i++) {
		start = rdtsc();
		end = rdtsc();
		sum = sum + (end - start);
	}

	cout << "read overhead is: " << ((double)sum) / ((double)(MAXITER)) << endl;
}


void CPU::loopOverhead() {
	uint64_t start;
	uint64_t end;

	rdtsc();
	uint64_t total_time = 0;
	start = rdtsc();
 	for (int i = 0; i < MAXITER; i++) {
	}
	end = rdtsc();
	total_time += end - start;

	cout << "loop overhead is: " << (double)total_time / MAXITER << endl;
}

