#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

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

	if((fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR)) == -1)	
	{
		perror("open");
		exit(2);
	}

	if ((file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		close(fd);
		exit(3);
	}

	close(fd);

	for (i = 0; i < 100; i++)
	{
		sscanf(file_memory, "%d", &integer);
		printf("Value retrieved: %3d\n", integer);
		sleep(1);
		sprintf((char *)file_memory, "%d\n", 2 * integer);
		sleep(1);
	}

	munmap((char *)file_memory, FILE_LENGTH);

	return 0;
}
