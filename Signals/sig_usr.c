#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigusr1_count = 0;

void handler()
{
	++sigusr1_count;
	printf("Trigger from Child.\n");
}
int main()
{
	struct sigaction act;
	int i, j;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &handler;
	sigaction(SIGUSR1, &act, NULL);

	if (fork()) // Parent
	{
		/* Do some lengthy stuff here. */
		printf("In the parent.\n");
		for(i = 0; i < 10; i++)
		{
			while (sleep(1)) // Sleep again - in case it gets signalled
				;
			printf("Brrrrrm\n");
		}
	}
	else // Child
	{
		for(i = 0; i < 100000; i++)
		{
			kill(getppid(), SIGUSR1);
		}
		printf("In a child.\n");
	}
	printf("SIGUSR1 was raised %d times\n", sigusr1_count);
	return 0;
}
