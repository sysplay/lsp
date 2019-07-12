#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

sig_atomic_t sigint_count = 0;
struct sigaction sa;

void handler(int signal_number)
{
	++sigint_count;
	//sa.sa_handler = SIG_DFL;
	//sigaction(SIGINT, &sa, NULL);
}
int main()
{
	int i;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	/* Do some lengthy stuff here. */
	for(i = 0; i < 10; i++)
	{
		printf("Grrrrrrrrr\n");
		sleep(1);
	}
	printf("SIG INT was raised %d times\n", sigint_count);
	return 0;
}
