#include <stdio.h>
#include <unistd.h>

int main()
{
	int a;

	printf("\nI am the back ground process, I will keep displaying my PID periodically\n");
	printf("\nYou will have to kill me if you want me to go away ...\n");

	while(1)
	{
		for(a = 1; a < 100000000; a++);
		printf("\nBackground process says HELLO ...");
		printf("\nPID: %d\tPPID: %d\n", (int)getpid(), (int)getppid());
	}

	return 0;
}
