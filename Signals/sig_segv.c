#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct sigaction sa_org;
int *ip = NULL;
int i;

void handler()
{
	static int warning_cnt = 0;

	warning_cnt++;
	printf("Hey!! don't fault on an invalid segment\n");
	if (warning_cnt == 10)
	{
		//ip = &i; // Still not working. See the assembly to understand
		printf("That's enough of warnings\n");
		signal(SIGSEGV, SIG_DFL);
	}
}

int main()
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handler;
	sigaction(SIGSEGV, &sa, &sa_org);
	
	*ip = 236;

	return 0;
}
