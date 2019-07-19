/*
 * This program demonstrates the concept of using barriers. Say you have n
 * independent threads, which would be working. But they all need to be working
 * only after all n have finished doing their initializations. So, barrier is
 * just the right choice
 */
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_WORKERS 4
#define RESOURCE_PER_WORKER 10

static pthread_barrier_t barrier;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int resource[RESOURCE_PER_WORKER * NUM_WORKERS];

void *worker(void *arg)
{
	int index = (int)(long)(arg);
	int i, done = 0;

	// Initalize my portion of resource
	printf("w%d: Initializing ...\n", index);
	for (i = 0; i < RESOURCE_PER_WORKER; i++)
	{
		resource[index + NUM_WORKERS * i] = i * (index + 1);
	}
	printf("w%d: Done\n", index);
	printf("w%d: Waiting for all threads to finish initialization ...\n", index);
	pthread_barrier_wait(&barrier);
	printf("w%d: Done\n", index);
	printf("w%d: Starting my task ...\n", index);
	sleep(5);
	while (!done)
	{
		i = rand() % (RESOURCE_PER_WORKER * NUM_WORKERS);
		pthread_mutex_lock(&mutex);
		printf("w%d: Value[%2d] = %2d\n", index, i, resource[i]);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	printf("w%d: Done\n", index);
	return NULL;
}

int main()
{
	pthread_t tid[NUM_WORKERS];
	int i;

	/* Mutex initialization has been done statically */
	pthread_barrier_init(&barrier, NULL, NUM_WORKERS);

	printf("main: Creating worker threads to start the work ...\n");
	for (i = 0; i < NUM_WORKERS; i++)
	{
		pthread_create(&tid[i], NULL, &worker, (void *)(long)i);
	}

	printf("main: Waiting for worker threads to finish the work ...\n");
	for (i = NUM_WORKERS - 1; i >= 0; i--)
	{
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);
	pthread_mutex_destroy(&mutex);

	printf("main: Completed\n");

	return 0;
}
