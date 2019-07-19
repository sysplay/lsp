#include <stdio.h>
#include <pthread.h>

void* thread_function(void* thread_arg)
{
	int i = 10;

	/* Do work here... */
	while (i--)
	{
		printf("Thread Working\n");
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_attr_t attr;
	pthread_t thread;

	pthread_attr_init(&attr);
	/* Commenting the line below doesn't really show any difference */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&thread, &attr, &thread_function, NULL);
	pthread_attr_destroy(&attr);
	/* Do work here... */
	while (1)
	{
		printf("Process Working\n");
		sleep(2);
	}
	/* No need to join the thread */
	return 0;
}
