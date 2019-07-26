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

static void process_eth(int fd)
{
	struct sockaddr_in from_addr;
	socklen_t sa_len;
	int read_cnt;
	char buf[BUF_SIZE];

	while (1)
	{
		sa_len = sizeof(from_addr);
		read_cnt = recvfrom(fd, buf, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &sa_len);
		if (read_cnt <= 0)
		{
			perror("recvfrom");
			continue;
		}
		buf[read_cnt - 1] = 0;
		printf("%s:%d: %s\n", inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port), buf);
	}
}

int main(int argc, char *argv[])
{
	int sock_fd;
	char *ip_addr;
	int yes;
	struct sockaddr_in my_addr;

	if (argc != 2)
	{
		printf("Usage: %s [ <ip addr> ]\n", argv[0]);
		return 1;
	}

	ip_addr = argv[1];

	printf("Creating socket ... ");
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0 /* IPPROTO_UDP */)) == -1)
	{
		perror("socket");
		return 2;
	}
	printf("done\n");

#if 0
	/* Get rid of "address already in use" error message */
	yes = 1;
	printf("Setting socket reusable ... ");
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		perror("setsockopt");
		return 3;
	}
	printf("done\n");
#endif

	my_addr.sin_family = AF_INET; // address family 
	my_addr.sin_port = htons(PORT_NUM); // short, network byte order
	if (ip_addr)
	{
		my_addr.sin_addr.s_addr = inet_addr(ip_addr);
	}
	else
	{
		my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // All IP addresses on this machine
	}
	bzero(&(my_addr.sin_zero), 8);

	if (ip_addr)
	{
		printf("Binding socket to %s ... ", ip_addr);
	}
	else
	{
		printf("Binding socket to all IP addresses on this system ... ");
	}
	if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("bind");
		close(sock_fd);
		return 4;
	}
	printf("done\n");

	process_eth(sock_fd);

	close(sock_fd);

	return 0;
}
