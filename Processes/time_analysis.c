/*
 * Try time ./time_analysis
 *
 * Observe: Total = Kernel (System) + User + Context Switch wastage (+ Sleep non-precision)
 *
 * Get more insight by trying combinations of commenting the read, nop, sleep logic
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int i, j;
	int fd;
	char c;

	if ((fd = open(argv[0], O_RDONLY)) == -1)
	{
		printf("Unable to open %s\n", argv[0]);
		return 1;
	}
	for (i = 0; i < 10; i++)
	{
		printf("Entry: %d\n", i);
		lseek(fd, 0, SEEK_SET);
		while (read(fd, &c, sizeof(char)) > 0)
			;
		for (j = 0; j < 1000000; j++)
		{
			asm volatile ("nop");
		}
		sleep(1);
	}
	close(fd);
	return 0;
}
