// Using fork() function

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	printf("Fork Demonstration\n");
	fork();
	printf("\nHello World\n");
	return 0;
}
