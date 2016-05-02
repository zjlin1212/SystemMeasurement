#ifndef _CPU_H_
#define _CPU_H_

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cstdint> 

#define MAXITER 100000


class CPU {
public:
	void readOverhead();
	void loopOverhead();
	void procedureOverhead();
	void syscallOverhead();
	void theadCreateOverhead();
	void processCreateOverhead();


};

#endif