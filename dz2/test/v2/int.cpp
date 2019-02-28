#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

const double IBEGIN = 0.0;
const double IEND = 1000000.0;
const double ISTEP = 0.001;

double a = 0;


double func(double x)
{
	return 1;
}


void integrate(double begin, double end, double step)
{
	double res = 0;
	double x = begin;

	while(x < end)
	{
		res += step * func(x);
		//printf("x = %f; res = %f\n", x, res);

		x += step;
	}

	a += res;
	
	printf("%f\n", res);
}


void *myThread(void *dummy)
{
	pthread_t mythid;
	mythid = pthread_self();

	double begin, end, step;

	begin = ((double *)dummy)[0];
	end = ((double *) dummy)[1];
	step = ((double *) dummy)[2];

	integrate(begin, end, step);

	printf("Thread %u, Calculation result = %f\n", (unsigned)mythid, a);
	
	return nullptr;
}


int main(int argc, char *argv[])
{
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

	double *data = new double[numb*3];
	pthread_t *thids = new pthread_t[numb];
	int result;

	double dist = (IEND - IBEGIN) / numb;

	for(int i = 0; i < numb; i++)
	{
		data[3*i] = IBEGIN + dist * i;
		data[3*i + 1] = data[3*i] + dist;
		data[3*i + 2] = ISTEP;

		result = pthread_create( &thids[i], (pthread_attr_t *)nullptr, myThread, (void *)(&data[i*3]));
		if(result)
		{
			printf("Error on thread1 create, return value = %d\n", result);
			exit(EXIT_FAILURE);
		}
		printf("Thread created, thids[%d] = %u\n", i, (unsigned)thids[i]);

		pthread_join(thids[i], (void **)nullptr);
	}

	printf("\na = %f\n", a);

	delete[] data;
	delete[] thids;

	return 0;
}

