#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


const double IBEGIN = 0.0;
const double IEND = 20000000.0;
const double ISTEP = 0.001;


struct IntSeg
{
	double begin;
	double end;
	double res;
	double arr[128]= {};
	int cpu;
	cpu_set_t set;
};


double func(double x);

int checkArg(int argc, char *argv[]);

void *myThread(void *dummy);

void integrate(IntSeg *seg);
void exitErr(const char *str);


int main(int argc, char *argv[])
{
	int numb = checkArg(argc, argv);
	double range = (IEND-IBEGIN) / numb;

	pthread_t thids[40];
	IntSeg intSegs[40];

	FILE *cpuinfo = popen("grep 'core id' /proc/cpuinfo && grep 'processor' /proc/cpuinfo", "r");
	if(!cpuinfo)
		exitErr("cpuinfo");

	char c;
	while((c = getc(cpuinfo)) != EOF)
		putchar(c);
	
	pclose(cpuinfo);

	int cpu_max = 7;
	int cpu = 0;
	for(int i = 0; i < numb; ++i)
	{
		if(cpu > cpu_max)
			cpu -= cpu_max + 1;
		printf("cpu = %d\n", cpu);
		
		intSegs[i].begin = IBEGIN + i*range;
		intSegs[i].end = IBEGIN + (i+1)*range;
		intSegs[i].res = 0;
		
		intSegs[i].cpu = cpu++;

		CPU_ZERO(&intSegs[i].set);
	}


	for(int i = 0; i < numb; ++i)
	{
		int checkErr = pthread_create( &thids[i], (pthread_attr_t *)nullptr, myThread, (void *) &intSegs[i]);
		if(checkErr)
		{
			printf("Error on thread1 create, return value = %d\n", checkErr);
			exit(EXIT_FAILURE);
		}
	}

	for(int i = 0; i < numb; ++i)
		pthread_join(thids[i], (void **)nullptr);

	double res = 0;

	for(int i = 0; i < numb; ++i)	
		res += intSegs[i].res;

	printf("\nres = %f\n", res);

	return 0;
}


double func(double x)
{
	return 1;
}


void integrate(IntSeg *seg)
{
	CPU_SET(seg->cpu, &seg->set);

	if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &seg->set) == -1)
		exitErr("setaffinity");

	register double x = seg->begin;
	register double end = seg->end;
	register double res = 0;
	register double step = ISTEP;

	while(x < end)
	{
		res += step * func(x);
		x += step;
	}

	seg->res = res;
}


int checkArg(int argc, char *argv[])
{
	if(argc != 2)
		exitErr("argc error");

	int numb = atoi(argv[1]);
	if((numb < 1) || (numb > 40))
		exitErr("argv error");

	return numb;
}


void *myThread(void *dummy)
{
	IntSeg *seg = (IntSeg *) dummy;

	integrate(seg);
	
	return nullptr;
}


void exitErr(const char *str)
{
	printf("exit with error : %s\n", str);
	exit(EXIT_FAILURE);
}

