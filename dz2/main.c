#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <math.h>


const double IBEGIN = 0.0;
const double IEND = 4000000.0;
const double ISTEP = 0.001;
const int CORE_POS = 11;
const int PROC_POS = 12;


typedef struct IntSeg
{
	double begin;
	double end;
	double res;
} IntSeg;


double func(double x) __attribute__((const));

int getCoreMaxNumb(void);
int getProcMaxNumb(void);
int checkArg(int argc, char *argv[]);

void *threadFunc(void *dummy);

void setSegs(IntSeg *intSegs, int numb, int th_numb);
void integrate(IntSeg *seg);
void exitErr(const char *str) __attribute__((noreturn));



int main(int argc, char *argv[])
{
	int numb = checkArg(argc, argv);
	printf("numb = %d\n", numb);

	pthread_t thids[40];
	IntSeg intSegs[40];

	getCoreMaxNumb();
	int cpu_max = getProcMaxNumb();
	int th_numb = cpu_max+1;
	if(numb > th_numb)
		th_numb = numb;

	setSegs(intSegs, numb, th_numb);

	cpu_set_t cpu_set;
	pthread_attr_t th_attr;
	pthread_attr_init(&th_attr);

	int cpu = 0;
	for(int i = 0; i < th_numb; i++)
	{
		if(cpu > cpu_max)
			cpu -= cpu_max+1;

		CPU_ZERO(&cpu_set);
		CPU_SET((unsigned long)cpu, &cpu_set);
		int checkErr = pthread_attr_setaffinity_np(&th_attr, sizeof(cpu_set_t), &cpu_set);
		if(checkErr)
			exitErr("pthread_attr_setaffinity_np error");

		checkErr = pthread_create( &thids[i], &th_attr, threadFunc, (void *)&intSegs[i]);
		if(checkErr)
		{
			printf("Error on thread[%d] create, return value = %d\n", i, checkErr);
			exit(EXIT_FAILURE);
		}

		cpu++;
	}

	pthread_attr_destroy(&th_attr);

	for(int i = 0; i < th_numb; i++)
		pthread_join(thids[i], (void **)NULL);

	double res = 0;

	for(int i = 0; i < numb; i++)	
		res += intSegs[i].res;

	printf("\nres = %f\n", res);
	
	return 0;
}



double func(double x)
{
	return sqrt(x);
}


int getCoreMaxNumb(void)
{
	FILE *coreinfo = popen("grep 'core id' /proc/cpuinfo", "r");
	if(!coreinfo)
		exitErr("coreinfo");

	int core_max = 0;
	char str[30] = "";
	char *checkError = NULL;
	while(1)
	{
		checkError = fgets(str, 30, coreinfo);
		if(!checkError)
			break;
		char firstn = str[CORE_POS];
		if((firstn < '0') || (firstn > '9'))
			exitErr("getCoreMaxNumb parse error");

		core_max = (int)(firstn - '0');
		char secondn = str[CORE_POS+1];
		if((secondn >= '0') && (secondn <= '9'))
		{
			core_max *= 10;
			core_max += (int)(secondn - '0');
		}
		printf("core id = %d\n", core_max);
	}
	
	pclose(coreinfo);

	printf("core_max = %d\n", core_max);

	return core_max;
}


int getProcMaxNumb(void)
{
	FILE *procinfo = popen("grep 'processor' /proc/cpuinfo", "r");
	if(!procinfo)
		exitErr("procinfo");

	int proc_max = 0;
	char str[30] = "";
	char *checkError = NULL;
	while(1)
	{
		checkError = fgets(str, 30, procinfo);
		if(!checkError)
			break;
		char firstn = str[PROC_POS];
		if((firstn < '0') || (firstn > '9'))
			exitErr("getCoreProcNumb parse error");

		proc_max = (int)(firstn - '0');
		char secondn = str[PROC_POS+1];
		if((secondn >= '0') && (secondn <= '9'))
		{
			proc_max *= 10;
			proc_max += (int)(secondn - '0');
		}
		printf("proc = %d\n", proc_max);
	}

	
	pclose(procinfo);

	printf("proc_max = %d\n", proc_max);
	return proc_max;	
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


void *threadFunc(void *dummy)
{
	IntSeg *seg = (IntSeg *)dummy;

	integrate(seg);
	
	return NULL;
}


void setSegs(IntSeg *intSegs, int numb, int th_numb)
{
	double range = (IEND-IBEGIN) / numb;

	int count = 0;
	for(int i = 0; i < th_numb; i++)
	{
		if(count == numb)
			count -= numb;
		
		intSegs[i].begin = IBEGIN + count*range;
		intSegs[i].end = IBEGIN + (count+1)*range;
		intSegs[i].res = 0;

		count++;
	}	
}


void integrate(IntSeg *seg)
{
	printf("thread[%lu], cpu numb = %d\n", pthread_self(), sched_getcpu());
	
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


void exitErr(const char *str)
{
	printf("exit with error : %s\n", str);
	exit(EXIT_FAILURE);
}
