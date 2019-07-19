#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define NUMTHREADS 3

void *threadfunc(void *parm)
{
	int no = (int)(long)parm;
	pthread_t self = pthread_self();
	int rc;

	printf("Thread %d (0x%x) entered\n", no, self);
	errno = 0;
	rc = sleep(30);
	if (rc != 0 && errno == EINTR)
	{
		printf("Thread %d (0x%x) got a signal delivered to it\n", no, self);
		return NULL;
	}
	printf("Thread %d (0x%x) did not get expected results!\n", no, self);
	return NULL;
}

void sighand(int signo)
{
	pthread_t self = pthread_self();

	printf("Thread (0x%x) signalled\n", self);
}

int main(int argc, char *argv[])
{
	struct sigaction actions;
	int i;
	pthread_t threads[NUMTHREADS];

	printf("main: Set up the alarm handler for the process\n");
	memset(&actions, 0, sizeof(actions));
	sigemptyset(&actions.sa_mask);
	actions.sa_handler = &sighand;

	if (sigaction(SIGALRM, &actions, NULL))
	{
		printf("main: Registering signal handler failed\n");
		return 1;
	}

	for (i = 0; i < NUMTHREADS; ++i)
	{
		if (pthread_create(&threads[i], NULL, &threadfunc, (void *)(long)i))
		{
			printf("main: Creating thread #%d failed\n", i);
			return 2;
		}
	}

	for (i = 0; i < NUMTHREADS; ++i)
	{
		sleep(3 - i);
		pthread_kill(threads[i], SIGALRM);
	}

	for (i = NUMTHREADS - 1; i >= 0; --i)
	{
		pthread_join(threads[i], NULL);
	}
	printf("main: Completed\n");

	return 0;
}
