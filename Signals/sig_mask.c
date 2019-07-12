#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

sig_atomic_t sigintcnt = 0;

void handler(int signum)
{
	++sigintcnt;
	if (signum == SIGUSR1)
	{
		printf("Sleeping ...\n");
		sleep(10);
		printf("Done\n");
	}
}

int main()
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler;

	if (sigaction(SIGINT, &sa, NULL) == -1)
		perror("Failed to install SIGINT signal handler");

	sigaddset(&sa.sa_mask, SIGINT);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
		perror("Failed to install SIGUSR1 signal handler");

	for(;;)
	{
		printf("Signal Count: %d\n", (int)sigintcnt);	
		sleep(5); // Would return when signalled
	}
	return 0;
}
