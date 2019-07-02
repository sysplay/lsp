#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;
	char buf[80];
	int cnt;

	if (argc != 2)
	{
		write(2, "Usage: system_cat <file>\n", strlen("Usage: system_cat <file>\n"));
		return 1;
	}
	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror("open");
		return 2;
	}
	while ((cnt = read(fd, buf, 80)) > 0)
	{
		write(1, buf, cnt);
	}
	close(fd);

	return 0;
}
