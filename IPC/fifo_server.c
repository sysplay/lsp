#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "fifo"

int main(void)
{
	FILE *fp;
	char readbuf[80];

	/* Create the FIFO if it does not exist */
	umask(0);
	mknod(FIFO_FILE, S_IFIFO | S_IRUSR | S_IWUSR, 0);

	printf("\nWaiting for writer clients ...\n");
	fp = fopen(FIFO_FILE, "r");
	while (fgets(readbuf, 80, fp))
	{
		printf("Received string: %s\n", readbuf);
	}
	fclose(fp);

	return 0;
}
