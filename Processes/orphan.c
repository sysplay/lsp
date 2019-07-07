#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid != 0)
	{
		printf("I am in parent block\n");
		/* This is the parent process. Exit immediately. */
		if (child_pid < 0)
		{
			exit(-child_pid);
		}
		else
		{
			//sleep(10);
			exit(0);
		}
	}
	else
	{
		printf("I am a child\n");
		sleep(30);
		printf("OMG! Orphan\n");
	}
	return 0;
}
