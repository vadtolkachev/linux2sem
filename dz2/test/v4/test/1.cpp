#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>


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

	while(x < end)
	{
		*res += step * func(x);
		x += step;
	}
}


int main()
{
	double a = 0;

	cpu_set_t set;
	CPU_ZERO(&set);

	CPU_SET(0, &set);

	if(sched_setaffinity(getpid(), sizeof(set), &set) == -1)
	{
		printf("sched_setaffinity\n");
		exit(EXIT_FAILURE);
	}

	integrate(IBEGIN, IEND, ISTEP, &a);

	printf("\na = %f\n", a);

	return 0;
}

