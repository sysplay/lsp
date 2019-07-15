#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define FIFO_FILE "fifo"

void broken_pipe(int sno)
{
	printf("Broken Pipe. Let's continue ...\n");
}

int main(int argc, char *argv[])
{
	int cnt = 1, i;
	struct sigaction sa;
	FILE *fp;


	if ((argc != 2) && (argc != 3))
	{
		printf("Usage: fifoclient <string> [ <cnt> ]\n");
		return 1;
	}
	if (argc == 3)
	{
		cnt = atoi(argv[2]);
	}

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &broken_pipe;
	sigaction(SIGPIPE, &sa, NULL);

	/* Create the FIFO if it does not exist */
	umask(0);
	mknod(FIFO_FILE, S_IFIFO | S_IRUSR | S_IWUSR, 0);

	printf("\nSending %d \"%s\" to %s ", cnt, argv[1], FIFO_FILE);
	fflush(stdout);
	if ((fp = fopen(FIFO_FILE, "w")) == NULL)
	{
		perror("fopen");
		return 2;
	}
	for (i = 0; i < cnt; i++)
	{
		fputs(argv[1], fp);
		fputs("\n", fp);
		fflush(fp);
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf(" done\n");
	fclose(fp);

	return 0;
}
