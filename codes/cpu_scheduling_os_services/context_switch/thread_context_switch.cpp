#include <iostream>

using namespace std;

// define some variables

// read the cycle
static inline uint64_t rdtsc(void) {
   uint32_t lo, hi;
   __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
   return (((uint64_t)hi << 32) | lo);
}
void *thread(void *vargp)
{
    pthread_exit(NULL);
}

void testthreadcontextswitch() {
    int* files;
   pipe(files);
   uint64_t start;
   uint64_t end;
   uint64_t total = 0;
   int repeatTime = 10;
   int forLoopOverhead = 6;
   pthread_t tid;
   int count = 0;

   // test process creation
   for (int i = 0; i < repeatTime; i++) {
       pipe(files);
       pthread_create(&tid,NULL,thread,NULL);
       start=rdtsc();
       pthread.join(tid,NULL);
       read(files[0],(void*)&end,sizeof(uint64_t));
       total += end - start;
       count += 1;
   }
   double result = (double) total / (double) count - forLoopOverhead;
   cout << "thread context switch = " << result << endl;

}

int main(int argc, const char* argv[]){
    int testTimes;
    for(int i=0;i<testTimes;i++){
        testthreadcontextswitch();
    }
}
