#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	char *file;
	int fd;
	struct flock lock;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		return 1;
	}
	file = argv[1];
	printf("Opening %s\n", file);
	if ((fd = open(file, O_RDWR)) == -1)
	{
		perror("open");
		return 1;
	}

	printf("\nLocking ... ");
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK; // Advisory lock
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0; // Whole file
	if (fcntl(fd, F_SETLKW, &lock) == -1)
	{
		perror("fcntl lock");
		return 2;
	}
	printf("Done. Hit <Enter> to unlock ...\n");
	getchar();
	printf("Unlocking ... ");
	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLKW, &lock) == -1)
	{
		perror("fcntl unlock");
		return 3;
	}
	printf("Done.\n");

	close(fd);
	return 0;
}
