#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static void sig_usr(int signo)
{
	if (signo == SIGUSR1) 
	{
		printf("\nRx: SIGUSR1\n");
	}
	else if (signo == SIGUSR2)
	{
		printf("\nRx: SIGUSR2\n");
	}
	else
	{
		printf("\nRx: Signal #:%d\n");
	}
}

int main()
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &sig_usr;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);

	printf("Waiting for signals ...\n");
	for ( ; ; ) pause();

	return 0;
}
