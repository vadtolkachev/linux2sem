#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>


const double IBEGIN = 0.0;
const double IEND = 5000000.0;
const double ISTEP = 0.001;


void get_current_time_with_ms(long *resms, time_t *ress)
{
    long            ms;
    time_t          s;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6);
    if (ms > 999) {
        s++;
        ms = 0;
    }
 
	*resms = ms;
	*ress = s; 

    //printf("Current: %ld.%03ld seconds\n", s, ms);
}


double func(double x)
{
	return 1;
}


void integrate(double begin, double end, double step, double *res)
{
	double x = begin;
	time_t t1s, t2s, ts;
	long t1ms, t2ms, tms;

	get_current_time_with_ms(&t1ms, &t1s);
	while(x < end)
	{
		*res += step * func(x);
		x += step;
	}
	get_current_time_with_ms(&t2ms, &t2s);
	ts = t2s - t1s;
	tms = t2ms - t1ms;

	if(tms < 0)
	{
		ts--;
		tms += 1000;
	}
	
	printf("res = %f\n", *res);
	printf("time: %ld.%03ld seconds\n", ts, tms);
}


void *myThread(void *dummy)
{
	double *res = (double *) dummy;

	integrate(IBEGIN, (IEND-IBEGIN)/2, ISTEP, res);

	//printf("th2 res = %f\n", *res);
	
	return nullptr;
}


int main()
{
	double a = 0;
	double res1[256];
	double res2[256];
	res1[0] = 0;
	res2[0] = 0;

	int result;
	
	pthread_t thid;

	result = pthread_create( &thid, (pthread_attr_t *)nullptr, myThread, (void *) res1);
	if(result)
	{
		printf("Error on thread1 create, return value = %d\n", result);
		exit(EXIT_FAILURE);
	}
	printf("Thread created, thid = %u\n", (unsigned)thid);
	
	integrate((IEND-IBEGIN)/2, IEND, ISTEP, res2);


	printf("th1 res = %f\n", *res2);

	pthread_join(thid, (void **)nullptr);
	a = *res1 + *res2;

	printf("\na = %f\n", a);

	return 0;
}

