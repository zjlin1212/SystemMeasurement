#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>


const size_t BLOCKSIZE = 4*1024;
const double CPUFREQ = 2.7;

using namespace std;

const  char* FILENAME[]={"file0","file1","file2","file3","file4","file5","file6","file7","file8","file9","file10","file11","file12","file13","file14","file15","file16","file17","file18","file19","file20","file21","file22","file23","file24","file25","file26","file27","file28","file29","file30","file31","file32"};

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}


double sequentialAccess(int fd,int id) {
    void* buffer = malloc(BLOCKSIZE);
    uint64_t start;
    uint64_t end;
    uint64_t total = 0;
    uint64_t count = 0;
    size_t result;
    
    while (true) {
        start = rdtsc();
        result = read(fd, buffer, BLOCKSIZE);
        end = rdtsc();
        
        if (result <= 0) {
            break;
        }
        
        count += 1;
        total += end - start;
    }
    
    return (double)total / (double)count * 1.0 / CPUFREQ / 1000;
}


// random access
double randomAccess(int fd,int id) {
    void* buffer = malloc(BLOCKSIZE);
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

int main(int argc, char *argv[]) {
    int  PROCESS_NUM=stoi(argv[1]);
    int fd[PROCESS_NUM];    
    pid_t pid[PROCESS_NUM];
    uint64_t total_time = 0;
        for (int i = 0; i < PROCESS_NUM; i++) {
            if((pid[i] = fork())==0){ //child
                fd[i] = open(FILENAME[i], O_SYNC);
                 // turn off cache
                if (fcntl(fd[i], F_NOCACHE, 1) != 0) {
                    cout << "turn off cache error" << endl;
                    return -1;
                }
                double seq_r=randomAccess(fd[i],i);
                cout << seq_r << endl;
                exit(0);
                
            }
        
        }
    return 0;
}





