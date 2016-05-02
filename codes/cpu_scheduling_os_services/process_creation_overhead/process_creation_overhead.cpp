#include <iostream>

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
   int repeatTime = 10;
   int forLoopOverhead = 6;
   pid_t pid;
   int count = 0;

   // test process creation
   for (int i = 0; i < repeatTime; i++) {
      start = rdtsc();
      pid = fork();

      if (pid != 0) {
         wait(NULL);
         end = rdtsc();
         total += end - start;
         count += 1;
      } else {
         exit(0);
      }
   }
   
   double result = (double) total / (double) count - forLoopOverhead;
   cout << "process creation time = " << result << endl;

   return 0;
}
