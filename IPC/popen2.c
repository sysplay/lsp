#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *pipein_fp, *pipeout_fp;
	char readbuf[80];

	/* Create one way pipe line with call to popen() */
	if ((pipein_fp = popen("ls", "r")) == NULL)
	{
		perror("popen ls:");
		exit(1);
	}
	/* Create one way pipe line with call to popen() */
	if ((pipeout_fp = popen("sort -r", "w")) == NULL)
	{
		perror("popen sort:");
		exit(1);
	}

	/* Processing loop */
	while (fgets(readbuf, 80, pipein_fp))
		fputs(readbuf, pipeout_fp);
	fflush(pipeout_fp);

	/* Close the pipes */
	pclose(pipein_fp);
	pclose(pipeout_fp);

	return 0;
}
