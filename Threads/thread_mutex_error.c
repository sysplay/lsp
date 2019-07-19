#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t mutex;

void *thread_function1(void *arg)
{
	printf("TF1: Thread fn one\n");
	pthread_mutex_lock(&mutex);
	printf("TF1: Acquired mutex\n");
	printf("TF1: Sleeping\n");
	sleep(8);
	printf("TF1: Out of sleep\n");
	//pthread_mutex_unlock(&mutex);
	return NULL;
}

void *thread_function2(void *arg)
{
	printf("TF2: Thread fn two\n");
	printf("TF2: Stuck on mutex forever\n");
	pthread_mutex_lock(&mutex);
	printf("TF2: Never reached");
	sleep(6);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main()
{
	pthread_t th_id1, th_id2;

	pthread_mutex_init(&mutex, NULL);

	printf("Creating thread1\n");
	pthread_create(&th_id1, NULL, &thread_function1, NULL);
	sleep(2);
	printf("Creating thread2\n");
	pthread_create(&th_id2, NULL, &thread_function2, NULL);

	pthread_join(th_id1, NULL);
	pthread_join(th_id2, NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}
