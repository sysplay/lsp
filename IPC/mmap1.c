#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int random_range(unsigned const low, unsigned const high)
{
	unsigned const range = high - low + 1;

	return low + (int)(((double)range) * rand() / (RAND_MAX + 1.0));
}

int main(int argc, char *const argv[])
{
	int fd;
	void *file_memory;
	int integer;
	int i;

	if (argc != 2)
	{
		printf("Usage: %s <file_for_mapping>\n", argv[0]);
		return 1;
	}

	srand(time(NULL));

	if ((fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
	{
		perror("open");
		exit(2);
	}
	lseek(fd, FILE_LENGTH + 1, SEEK_SET);

	write(fd, "", 1);
	lseek(fd, 0, SEEK_SET);

	if ((file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		close(fd);
		exit(3);
	}

	close(fd);

	for (i = 0; i < 100; i++)
	{
		sprintf((char *)file_memory, "%d\n", random_range(-100, 100));
		sleep(1);
		sscanf(file_memory, "%d", &integer);
		printf("Value stored: %3d\n", integer);
		sleep(1);
	}

	munmap(file_memory, FILE_LENGTH);

	return 0;
}
