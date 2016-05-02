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
    int* files;
   pipe(files);
   uint64_t start;
   uint64_t end;
   uint64_t total = 0;
   int repeatTime = 10;
   int forLoopOverhead = 6;
   pid_t pid;
   int count = 0;

   // test process creation
   for (int i = 0; i < repeatTime; i++) {
      if ((pid = fork())!= 0) {
         start = rdtsc();
         wait(NULL);
         read(files[0],(void*)&end,sizeof(uint64_t));
      } else {
         end = rdtsc();
         write(files[1],(void*)&end,sizeof(uint64_t));
         exit(0);
      }
         total += end - start;
         count += 1;
   }
   
   double result = (double) total / (double) count - forLoopOverhead;
   cout << "process context switch overhead = " << result << endl;

   return 0;
}
