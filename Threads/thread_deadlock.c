#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t mutex1, mutex2;

void *thread_function1(void *arg)
{
	printf("TF1: Acquiring mutex 1 ... ");
	fflush(stdout);
	pthread_mutex_lock(&mutex1);
	printf("done\n");
	printf("TF1: Doing some task ...\n");
	sleep(1);
	printf("TF1: Acquiring mutex 2 ... (blocked)\n");
	pthread_mutex_lock(&mutex2);
	printf("TF1: Done\n");
	printf("TF1: Task Done\n");
	pthread_mutex_unlock(&mutex2);
	pthread_mutex_unlock(&mutex1);
	return NULL;
}

void *thread_function2(void *arg)
{
	printf("TF2: Acquiring mutex 2 ... ");
	fflush(stdout);
	pthread_mutex_lock(&mutex2);
	printf("done\n");
	printf("TF2: Doing some task ...\n");
	sleep(1);
	printf("TF2: Acquiring mutex 1 ... (blocked)\n");
	pthread_mutex_lock(&mutex1);
	printf("TF2: Done\n");
	printf("TF2: Task Done\n");
	pthread_mutex_unlock(&mutex1);
	pthread_mutex_unlock(&mutex2);
	return NULL;
}

int main()
{
	pthread_t th_id1, th_id2;

	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);

	printf("Creating thread1\n");
	pthread_create(&th_id1, NULL, &thread_function1, NULL);
	printf("Creating thread2\n");
	pthread_create(&th_id2, NULL, &thread_function2, NULL);

	pthread_join(th_id1, NULL);
	pthread_join(th_id2, NULL);

	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex1);

	return 0;
}
