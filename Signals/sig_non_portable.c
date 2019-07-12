#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig)
{
	printf("\nOUCH! I got the signal: %d\n", sig);
	signal(SIGINT, SIG_DFL);
}

int main()
{
	signal(SIGINT, ouch);
	while(1)
	{
		printf("Hello World\n");
		sleep(1);
	}
	return 0;
}
