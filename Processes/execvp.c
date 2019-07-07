#include <stdio.h>
#include <sys/types.h>

/*Here, we don't need to pass the path name. The execvp() takes the default path*/

int main()
{
	char *temp[3];
	temp[0] = "ls";
	temp[1] = "-l";
	temp[2] = NULL;
	execvp("ls", temp);
	printf("This will not print\n");
	return 0;
}
