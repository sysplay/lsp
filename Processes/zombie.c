#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int cpid, status;

	if ((cpid = fork()) != 0)
	{
		printf("\nParent sleeping\n");
		sleep(50);
		if (cpid > 0)
		{
			wait(&status); // TO TRY: Comment out this line. Then, who cleans the zombie?
		}
		printf("\nParent over\n");
	}
	else
	{
		printf("\nChild done\n");
	}
	return 0;
}
