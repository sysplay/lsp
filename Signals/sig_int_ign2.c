#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigint_count = 0;
void handler(int signal_number)
{
	++sigint_count;
	signal(SIGINT, SIG_IGN);
}

int main()
{
	int i;
	struct sigaction sa;
	
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler;
	sigaction(SIGINT, &sa, NULL);
	
	for(i = 0; i < 10; i++)
	{
		printf("He Ha Ha\n");
		sleep(1);
	}
	
	printf("\nSIGINT was raised %d times\n", sigint_count);
	return 0;
}
