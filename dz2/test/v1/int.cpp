#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>


double func(double x)
{
	return x;
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

	printf("%f\n", res);
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

	integrate(0, 500000, 0.001);

	return 0;
}

