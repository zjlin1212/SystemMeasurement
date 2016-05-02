#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>

using namespace std;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

int main(int argc, const char* argv[]) {
   int fd = open(argv[1], O_RDWR);

   if (fd == -1) {
      cout << "read file error" << endl;
      return -1;
   }
   
   // init
   uint64_t start;
   uint64_t end;
   uint64_t total = 0;
   off_t offset = atoll(argv[1]);
   off_t fileSize = lseek(fd, 0, SEEK_END);
   char* map = (char*) mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

   // begin test
   char c;
   
   start = rdtsc();
   for (int i = 0; i < 200; i++) {
      c = map[(((i + 1) * offset) % (fileSize - 1))];
   }
   end = rdtsc();
   total += end - start;

   // calculate avg, delete read cycle and rdt cycle
   double avg = (double)(total - 106) / 100 - 6;
   cout << "average time" << avg << endl;
   munmap(map, fileSize);
   close(fd);

   return 0;
}
