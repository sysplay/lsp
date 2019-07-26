#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>
#include <sys/select.h>

#define BUF_SIZE 80

#define FIFO_FILE "pipe_win"

#define INPUT_FD 0
#define OUTPUT_FD 1

int read_till_nl(int fd, char *buf, int buf_size)
{
	int i = 0;

	while ((read(fd, buf + i, 1) == 1) && (i < (buf_size - 1)) && (buf[i] != '\n'))
	{
		i++;
	}
	
	if ((i < buf_size) && (buf[i] != '\n'))
	{
		return -1;
	}
	else
	{
		buf[i++] = '\n';
		buf[i] = 0;
		return i;
	}
}

int main(int argc, char *argv[])
{
	char *rd_file, *wr_file;
	int rfd, wfd, max_fd;
	fd_set fds;
	char readbuf[BUF_SIZE];
	int read_cnt;

	if (strstr(argv[0], "pipe_window0"))
	{
		rd_file = FIFO_FILE "1";
		wr_file = FIFO_FILE "0";
		printf("Program 0\n");
	}
	else if (strstr(argv[0], "pipe_window1"))
	{
		rd_file = FIFO_FILE "0";
		wr_file = FIFO_FILE "1";
		printf("Program 1\n");
	}
	else
	{
		printf("Usage: pipe_window0 or pipe_window1\n");
		return 1;
	}

	/* Create the FIFO if it does not exist */
	umask(0);
	mknod(FIFO_FILE "0", S_IFIFO | S_IRUSR | S_IWUSR, 0);
	mknod(FIFO_FILE "1", S_IFIFO | S_IRUSR | S_IWUSR, 0);

	if ((rfd = open(rd_file, O_RDWR)) == -1)
	{
		printf("Error opening the FIFO for reading\n");
		return 2;
	}
	if ((wfd = open(wr_file, O_RDWR)) == -1)
	{
		printf("Error opening the FIFO for writing\n");
		return 3;
	}

	printf("Type in content to transfer over a pipe. ^D to exit.\n");
	printf("> ");
	fflush(stdout);
	FD_ZERO(&fds);
	FD_SET(INPUT_FD, &fds);
	FD_SET(rfd, &fds);
	max_fd = ((rfd > INPUT_FD) ? rfd : INPUT_FD) + 1;

	while (select(max_fd, &fds, NULL, NULL, NULL) > 0)
	{
		if (FD_ISSET(INPUT_FD, &fds))
		{
			read_cnt = read_till_nl(INPUT_FD, readbuf, BUF_SIZE);
			if (read_cnt < 0)
			{
				break;
			}
			write(wfd, readbuf, read_cnt);
			printf("> ");
			fflush(stdout);
		}
		if (FD_ISSET(rfd, &fds))
		{
			read_cnt = read_till_nl(rfd, readbuf, BUF_SIZE);
			if (read_cnt < 0)
			{
				break;
			}
			write(OUTPUT_FD, "\nReceived: ", 11);
			write(OUTPUT_FD, readbuf, read_cnt);
			write(OUTPUT_FD, "> ", 2);
		}

		FD_SET(INPUT_FD, &fds);
		FD_SET(rfd, &fds);
	}

	close(wfd);
	close(rfd);

	return 0;
}
