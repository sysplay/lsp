#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT_NUM 3000
#define BUF_SIZE 100

static void transmit_eth(int fd, char *to_ip)
{
	struct sockaddr_in my_addr;
	int read_cnt, write_cnt;
	char buf[BUF_SIZE];

	my_addr.sin_family = AF_INET; // address family 
	my_addr.sin_port = htons(PORT_NUM); // short, network byte order
	my_addr.sin_addr.s_addr = inet_addr(to_ip);
	bzero(&(my_addr.sin_zero), 8);

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
		if ((write_cnt = sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&my_addr, sizeof(my_addr))) == -1)
		// May generate SIGPIPE, if server closes the connection
		{
			perror("sendto");
		}
	}
}

int main(int argc, char *argv[])
{
	int sock_fd;
	struct sockaddr_in my_addr;

	if (argc != 2)
	{
		printf("Usage: %s <server ip addr>\n", argv[0]);
		return 1;
	}

	printf("Creating socket ... ");
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0 /* IPPROTO_UDP */)) == -1)
	{
		perror("socket");
		return 2;
	}
	printf("done\n");

	transmit_eth(sock_fd, argv[1]);

	close(sock_fd);

	return 0;
}
