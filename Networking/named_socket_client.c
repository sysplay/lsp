#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MY_SOCK_PATH "/tmp/ns"
#define BUF_SIZE 100

static void transmit_eth(int fd)
{
	int read_cnt, write_cnt;
	char buf[BUF_SIZE];

	while (1)
	{
		printf("Enter string to be transmitted (^D to exit): ");
		read_cnt = scanf("%[^\n]%*c", buf);
		if (read_cnt == EOF)
		{
			break;
		}
		if (read_cnt == 0)
		{
			scanf("%*c");
			buf[0] = 0;
		}
		if ((write_cnt = send(fd, buf, strlen(buf) + 1, 0)) == -1)
		// May generate SIGPIPE, if server closes the connection
		{
			perror("send");
		}
	}
}

int main(int argc, char *argv[])
{
	int sock_fd;
	struct sockaddr_un my_addr;

	printf("Creating socket ... ");
	if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0 /* IPPROTO_TCP */)) == -1)
	{
		perror("socket");
		return 2;
	}
	printf("done\n");

	my_addr.sun_family = AF_UNIX; // address family 
	strncpy(my_addr.sun_path, MY_SOCK_PATH, sizeof(my_addr.sun_path) - 1);

	printf("Connecting socket to %s ... ", MY_SOCK_PATH);
	if (connect(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("connect");
		close(sock_fd);
		return 4;
	}
	printf("done\n");

	transmit_eth(sock_fd);

	close(sock_fd);

	return 0;
}
