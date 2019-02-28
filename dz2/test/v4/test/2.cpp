#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
//#include <sched.h>


const double IBEGIN = 0.0;
const double IEND = 5000000.0;
const double ISTEP = 0.001;


struct IntSeg
{
	double begin;
	double end;
	double res;
	//double arr[128];
	cpu_set_t *set;
};


int checkArg(int argc, char *argv[]);

double func(double x);

void *myThread(void *dummy);

void integrate(IntSeg *seg);

void exitErr(const char *str);


int main()
{
	double a = 0;

	cpu_set_t set;
	CPU_ZERO(&set);

	IntSeg seg1 = { IBEGIN, (IEND-IBEGIN)/2, 0, &set };
	IntSeg seg2 = { (IEND-IBEGIN)/2, IEND, 0, &set };
	
	pthread_t thid;

	int result = pthread_create( &thid, (pthread_attr_t *)nullptr, myThread, (void *) &seg1);
	if(result)
	{
		printf("Error on thread1 create, return value = %d\n", result);
		exit(EXIT_FAILURE);
	}

	printf("Thread created, thid = %u\n", (unsigned)thid);


	CPU_SET(0, seg2.set);
	
	integrate(&seg2);

	pthread_join(thid, (void **)nullptr);
	a = seg1.res + seg2.res;

	printf("\na = %f\n", a);

	return 0;
}


double func(double x)
{
	return 1;
}


void integrate(IntSeg *seg)
{
	if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), seg->set) == -1)
		exitErr("sched_setaffinity");

	double x = seg->begin;

	while(x < seg->end)
	{
		seg->res += ISTEP * func(x);
		x += ISTEP;
	}
}


void *myThread(void *dummy)
{
	IntSeg *seg = (IntSeg *) dummy;

	CPU_SET(1, seg->set);

	integrate(seg);
	
	return nullptr;
}


void exitErr(const char *str)
{
	printf("%s\n", str);
	exit(EXIT_FAILURE);
}

