#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "indian_rw"

int main()
{
	char s[300];
	int fd, ret, num;

	if (mknod(FIFO_NAME, S_IFIFO | S_IRUSR | S_IWUSR, 0) == -1)
	{
		perror("mknod");
	}

	printf("Waiting for readers ...\n");
	if ((fd = open(FIFO_NAME, O_WRONLY)) == -1)
	{
		perror("open");
		return 1;
	}
	printf("Got a reader - Type some stuff\n");

	while ((ret = scanf("%[^\n]%*c", s)), !feof(stdin)) 
	{
		if (ret == 0) // No input read - remove the newline
		{
			getchar();
			continue;
		}
		if ((num = write(fd, s, strlen(s))) == -1)
		{
			perror("write");
		}
		else
		{
			printf("speak: wrote %d bytes\n", num);
		}
	}

	close(fd);

	return 0;
}
