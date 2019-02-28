#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>


void get_current_time_with_ms(long *resms, time_t *ress)
{
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999) {
        s++;
        ms = 0;
    }
 
	*resms = ms;
	*ress = s; 

    printf("Current time: %ld.%03ld seconds\n", s, ms);
}


const double IBEGIN = 0.0;
const double IEND = 5000000.0;
const double ISTEP = 0.001;


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
		//printf("x = %f; res = %f\n", x, res);

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
	
	printf("res%f\n", *res);
	printf("Current time: %ld.%03ld seconds\n", ts, tms);
}


int main()
{
	double a = 0;

	integrate(IBEGIN, IEND, ISTEP, &a);

	printf("\na = %f\n", a);

	return 0;
}

