#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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

// sequential access
double sequentialAccess(int fd, void* buffer) {
   uint64_t start;
   uint64_t end;
   uint64_t total = 0;
   uint64_t count = 0;
   size_t result;

   while (true) {
      start = rdtsc();
      result = read(fd, buffer, BLOCKSIZE);
      end = rdtsc();

      if (result == 0) {
         break;
      }

      count += 1;
      total += end - start;
   }

   return (double)total / (double)count * 1.0 / CPUFREQ / 1000;
}


// random access
double randomAccess(int fd, void* buffer) {
   uint64_t start;
   uint64_t end;
   uint64_t total = 0;
   uint64_t count = 0;
   size_t result;
   off_t fileSize = lseek(fd, 0, SEEK_END);
   off_t blockCount = fileSize / BLOCKSIZE;

   // repeat read
   for (int i = 0; i < blockCount; i++) {
      // random choose a block
      off_t offset = rand() % blockCount;
      
      start = rdtsc();
      lseek(fd, offset * BLOCKSIZE, SEEK_SET);
      result = read(fd, buffer, BLOCKSIZE);
      end = rdtsc();

      count += 1;
      total += end - start;
   }

   return (double)total / (double)count * 1.0 / CPUFREQ / 1000;
}

// random access

int main(int argc, const char* argv[]) {
   // init
   int fd = open(argv[1], O_SYNC);

   // turn off cache
   if (fcntl(fd, F_NOCACHE, 1) != 0) {
      cout << "turn off cache error" << endl;
   }

   void* buffer = malloc(BLOCKSIZE);

   // sequential read
   double seq_r = sequentialAccess(fd, buffer);
   // random read
   double ran_r = randomAccess(fd, buffer);

   cout << "seq result(us) = " << seq_r << endl;
   cout << "ran result(us) = " << ran_r << endl;

   close(fd);
   return 0;
}
