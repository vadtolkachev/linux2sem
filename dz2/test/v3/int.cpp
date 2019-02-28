#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

const double IBEGIN = 0.0;
const double IEND = 1000000.0;
const double ISTEP = 0.001;


struct IntSeg
{
	double begin;
	double end;
	double step;
	double res;
};


double func(double x)
{
	return 1;
}


void integrate(double begin, double end, double step, double *res)
{
	double x = begin;
	*res = 0;

	while(x < end)
	{
		*res += step * func(x);
		//printf("x = %f; res = %f\n", x, res);

		x += step;
	}
	
	printf("%f\n", *res);
}


void *myThread(void *dummy)
{
	pthread_t mythid;
	mythid = pthread_self();

	IntSeg *seg = (IntSeg *) dummy;

	integrate(seg->begin, seg->end, seg->step, &seg->res);

	printf("Thread %u, Calculation result = %f\n", (unsigned)mythid, seg->res);
	
	return nullptr;
}


int main(int argc, char *argv[])
{
	double a = 0;

	if(argc != 2)
	{
		printf("argc error\n");
		exit(EXIT_FAILURE);
	}

	int numb = atoi(argv[1]);
	if((numb < 1) || (numb > 40))
	{
		printf("argv error\n");
		exit(EXIT_FAILURE);
	}

	printf("numb = %d\n", numb);

	IntSeg *data = new IntSeg[numb];
	pthread_t *thids = new pthread_t[numb];
	int result;

	double dist = (IEND - IBEGIN) / numb;

	for(int i = 0; i < numb - 1; i++)
	{
		data[i].begin = IBEGIN + dist * i;
		data[i].end = data[i].begin + dist;
		data[i].step = ISTEP;

		result = pthread_create( &thids[i], (pthread_attr_t *)nullptr, myThread, (void *)(&data[i]));
		if(result)
		{
			printf("Error on thread1 create, return value = %d\n", result);
			exit(EXIT_FAILURE);
		}
		printf("Thread created, thids[%d] = %u\n", i, (unsigned)thids[i]);

		pthread_join(thids[i], (void **)nullptr);
		
		a += data[i].res;
	}

	double mres = 0;
	
	integrate(IEND - dist, IEND, ISTEP, &mres);
	a += mres;

	printf("\na = %f\n", a);

	delete[] data;
	delete[] thids;

	return 0;
}

