#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t child_pid;

	printf("The main program process id is %d\n", (int)getpid());
	child_pid = fork();
	if (child_pid != 0)
	{
 		printf("The parent process id is %d\n", (int)getppid());
		printf("This is the parent process, with id %d\n", (int)getpid());
		printf("The child’s process id is %d\n", (int)child_pid);
	}
	else
	{
 		printf("The parent process id is %d\n", (int)getppid());
		printf("This is the child process, with id %d\n", (int)getpid());
	}
	return 0;
}
