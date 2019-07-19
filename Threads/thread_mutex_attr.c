#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t mutex;

void *thread_function(void *arg)
{
	int err;

	printf("Thread fn one\n");
	pthread_mutex_lock(&mutex);
	printf("Acquired mutex\n");
	printf("Acquiring again ... ");
	fflush(stdout);
	err = pthread_mutex_lock(&mutex);
	if (err == 0)
	{
		printf("done\n");
	}
	else
	{
		printf("failed with error #%d\n", err);
	}
	printf("Sleeping\n");
	sleep(8);
	printf("Out of sleep\n");
	pthread_mutex_unlock(&mutex);
	printf("Released mutex\n");
	if (err == 0)
	{
		pthread_mutex_unlock(&mutex);
		printf("Released again\n");
	}
	return NULL;
}


int main()
{
	pthread_t th_id;
	pthread_mutexattr_t attr;

	pthread_mutexattr_init(&attr);
	/* TRY: Various attr types below to see the effect. Default is PTHREAD_MUTEX_NORMAL */
	//pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	//pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&mutex, &attr);
	pthread_mutexattr_destroy(&attr);

	printf("Creating thread\n");
	pthread_create(&th_id, NULL, &thread_function, NULL);

	pthread_join(th_id, NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}
