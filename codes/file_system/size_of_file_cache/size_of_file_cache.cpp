#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

// define some variables
const size_t BLOCKSIZE = 4 * 1024;
const double CPUFREQ = 2.7;

// read the cycle
static inline uint64_t rdtsc(void) {
   uint32_t lo, hi;
   __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
   return (((uint64_t)hi << 32) | lo);
}

int main(int argc, const char* argv[]) {
   // read file into the memory
   FILE* fs = fopen(argv[1], "r");
   char buffer[BLOCKSIZE];
   size_t result;

   int cc = 0;
   while((result = fread(buffer, 1, BLOCKSIZE, fs)) > 0) {
      cc += 1;
   }

   // close it and reopen
   fclose(fs);
   fs = fopen(argv[1], "r");
   

   // reopen it bnd read it
   uint64_t start;
   uint64_t end;
   uint64_t total = 0   ;
   uint64_t count = 0;

   while (true) {
      start = rdtsc();
      result = fread(buffer, 1, BLOCKSIZE, fs);
      end = rdtsc();

      if (result == 0) {
         break;
      }

      total += end - start;
      count += 1;
   }    

   cout << "avg time (us)= " << (double)total / (double)count * 1.0 / CPUFREQ / 1000 << endl;
   return 0;
}
