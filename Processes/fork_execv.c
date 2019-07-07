#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int sat(char *prog, char **arg)
{
	pid_t sa;

	sa = fork();
	if (sa != 0)
		return sa;
	else
	{
		execv(prog, arg);
		fprintf(stderr, "An error occured");
		abort();
	}
}

int main()
{
	char *agr[] = {"ls", "-l", "/", NULL};

	if (fork() == 0)
	{
		sat("/bin/ls", agr);
		printf("\nThis will not print, if all ok\n");
	}
	else
	{
		sleep(1);
		printf("\nDone with main program.\n");
	}
	return 0;
}
