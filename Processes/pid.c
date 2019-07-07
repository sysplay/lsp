#include <stdio.h>
#include <unistd.h>

// Invoke this program several times & observe the Process Id and Parent Process Id
// Invoke from different shell & observe the Parent Process Id

int main()
{

 	printf("The process id is %d\n", (int)getpid());
 	printf("The parent process id is %d\n", (int)getppid());
	return 0;
}
