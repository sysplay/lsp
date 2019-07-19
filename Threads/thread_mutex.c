/*
 * This program performs a dot product with concurrent threads. The main data is
 * made available to all threads through a globally accessible structure. Each
 * thread works on a different part of the data, and computes sum into a shared
 * part, protected by a global mutex. The main thread waits for all the threads
 * to complete their computations, and then it prints the resulting sum.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100
#define NUMTHRDS 4 // Should be a factor of VECLEN

typedef struct
{
	double a[VECLEN];
	double b[VECLEN];
	double sum;
} VectorData;

/* Define globally accessible variables and a mutex */
static VectorData vect_data;
static pthread_mutex_t mutex_sum;

/*
 * The function dot_prod is activated when the thread is created. As before, all
 * input to this routine is obtained from a structure of type VectorData and all
 * output from this function is written into this structure. The benefit of this
 * approach is apparent for the multi-threaded program: when a thread is created
 * we pass a single argument to the activated function - typically a thread
 * number. All  the other information required by the function is accessed from
 * the globally accessible structure.
 */
void *dot_prod(void *arg)
{
	int offset = (int)(long)arg;
	int len, i;
	double *x, *y, mysum;

	len = VECLEN / NUMTHRDS;
	x = vect_data.a + offset * len;
	y = vect_data.b + offset * len;

	mysum = 0;
	for (i = 0; i < len; i++)
	{
		mysum += (x[i] * y[i]);
	}

	pthread_mutex_lock(&mutex_sum);
	vect_data.sum += mysum;
	pthread_mutex_unlock(&mutex_sum);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i;
	pthread_t tid[NUMTHRDS];
	void *status;

	/* Initialize values by uniformly distributed random numbers */
	srand48((long)(status));
	for (i = 0; i < VECLEN; i++)
	{
		vect_data.a[i] = drand48() * (VECLEN / 2);
		vect_data.b[i] = drand48() * (VECLEN / 2);
	}
	vect_data.sum = 0;

	pthread_mutex_init(&mutex_sum, NULL);

	for (i = 0; i < NUMTHRDS; i++)
	{
		pthread_create(&tid[i], NULL, &dot_prod, (void *)(long)i);
	}

	/* Wait on the other threads */
	for (i = 0; i < NUMTHRDS; i++)
	{
		pthread_join(tid[i], &status);
	}

	pthread_mutex_destroy(&mutex_sum);

	/* After joining, print out the results */
	printf("Sum = %lf\n", vect_data.sum);

	return 0;
}
