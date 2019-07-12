#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigint_count = 0;
struct sigaction sa_org;

void handler()
{
	sigint_count++;
	//sigaction(SIGINT, &sa_org, NULL);
}

int main()
{
	int i;
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler;
	sigaction(SIGINT, &sa, &sa_org);
	
	for(i = 0; i < 20; i++)
	{
		printf("Hmmmmm\n");
		sleep(1);
	}
	
	printf("\nSIGINT was raised %d times\n", sigint_count);
	return 0;
}
