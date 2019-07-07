#include <stdio.h>
#include <sys/types.h>

int main()
{
	pid_t pid;

	printf("1: This process id:%d\n", getpid());
	fork();
	printf("2: This process id:%d\n", getpid());
	fork();
	printf("3: This process id:%d\n", getpid());
	fork();
	printf("4: This process id:%d; Parent:%d\n", getpid(), getppid());
	sleep(1);
	printf("Hello World\n");
	return 0;
}
