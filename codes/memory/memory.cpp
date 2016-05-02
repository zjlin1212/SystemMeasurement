#include "memory.h"
#include "mem_bandwidth.h"
#include <iostream>
#define CPU_FREQUENCY 2899102925
using namespace std;

void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
 
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
 
        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}

void memory::RAMaccessTime(){
	for(int i=2; i< 20; i ++){
		cout << "log2:" << i << ":  \t" << measureRAMaccess(1 << i , 1 << 12) << endl;
	}
}

void memory::RAMbandwidth(){

	cout << measureBandwidth() << endl;
	// memWriteTest(src);
}

void memory::pageFault(){
	
}

double memory::measureBandwidth(){
	int64_t start;
    int64_t end;
    double bandwidth;
    double res=0;
    int64_t total_time = 0;
	uint64* src = (uint64*)malloc(DATA_SIZE);
	memset( src, 1 , DATA_SIZE);
	for(int i=0;i<10; i++){
	start = rdtsc();
	//memWriteTest(src);
	memReadTest(src);
	end   = rdtsc();
	total_time = end - start;
	bandwidth = (double)DATA_SIZE / ((double)total_time / (double)CPU_FREQUENCY) / (double)(1 << 20);
	if(res < bandwidth)
		res = bandwidth;
	}
    //double ans = (total_time - 106)*1.0 / (1<<30) - 6;
	free(src);
    return res;
}

double memory::measureRAMaccess(int size, int stride){
	int *A = (int *)malloc(size*1024);
	int strideLength = 64;
	int num, length;
    length = strideLength / 4;   
    num = size * 1024 / 4;
    int index;
    A[0] = 0;
    for (int i = 0; i < num; i++)
    	A[i] = i;
    for(int j=0; j< num/stride; j++)
    	for(int i=0; i<stride && i < num; i++)
        	A[i+j*stride] = i+(j+1)*stride;
    for(int i=0; i<stride; i++)
    	A[stride*(num/stride) + i] = i+1;
    randomize ( A, num );
    int x = 0;
    int64_t start;
    int64_t end;
    int64_t total_time = 0;
    int count = 50000;

    start = rdtsc();
    for (int i = 0; i < count; i++) {
        x = A[x];
    }
    end = rdtsc();
    total_time = end - start;
    double ans = (total_time - 106)*1.0 / count - 6;
    free(A);
    return ans;

    for(int i=0; i<size*1024; i++)
    	cout << A[i] << " ";
    cout << endl;

}

