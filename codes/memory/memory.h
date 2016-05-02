#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cstdint> 

#define MAXITER 100000

static inline uint64_t rdtsc(void) {
	uint32_t lo, hi;
	__asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
	return (((uint64_t)hi << 32) | lo);
}



class memory {
public:
	void RAMaccessTime();
	void RAMbandwidth();
	void pageFault();
private:
	double measureRAMaccess(int size, int stride);
	double measureBandwidth();
};

#endif