#include <iostream>
#include <ctime>

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

   // test get pid
   for (int i = 0; i < repeatTime; i++) {
      start = rdtsc();
      getppid();
      end = rdtsc();
      total += end - start;
   }
   
   double result = (double) total / (double) repeatTime - forLoopOverhead;
   cout << "get pid time = " << result << endl;

   // test time()
   total = 0;
   for (int i = 0; i < repeatTime; i++) {
      start = rdtsc();
      time(0);
      end = rdtsc();
      total += end - start;
   }
   
   result = (double) total / (double) repeatTime - forLoopOverhead;
   cout << "get time() = " << result << endl;

   return 0;
}
