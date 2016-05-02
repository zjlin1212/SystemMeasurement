#include <iostream>
#include <pthread.h>

using namespace std;

// define some variables

// read the cycle
static inline uint64_t rdtsc(void) {
   uint32_t lo, hi;
   __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
   return (((uint64_t)hi << 32) | lo);
}

int main(int argc, const char* argv[]) {
   uint64_t start;
   uint64_t end;
   uint64_t total = 0;
   int repeatTime = 100000;
   int forLoopOverhead = 6;
   pthread_t pt;

   // test thread creation
   for (int i = 0; i < repeatTime; i++) {
      start = rdtsc();
      pthread_create(&pt, NULL, NULL, NULL);
      pthread_join(pt, NULL);
      end = rdtsc();
      total += end - start;
   }
   
   double result = (double) total / (double) repeatTime - forLoopOverhead;
   cout << "thread creation time = " << result << endl;

   return 0;
}
