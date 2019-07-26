#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT_NUM 2000
#define Q_LEN 1
#define BUF_SIZE 100

static void process_eth(int fd, char *from_ip)
{
	int read_cnt;
	char buf[BUF_SIZE];

	while (1)
	{
		if ((read_cnt = recv(fd, buf, BUF_SIZE, 0)) <= 0)
		{
			printf("Connection from %s went down\n", from_ip);
			break;
		}
		buf[read_cnt - 1] = 0;
		printf("%s: %s\n", from_ip, buf);
	}
}

int main(int argc, char *argv[])
{
	int sock_fd;
	int yes;
	char *ip_addr;
	struct sockaddr_in my_addr;
	int eth_fd;
	socklen_t sin_size;
	struct sockaddr_in their_addr;

	if (argc != 2)
	{
		printf("Usage: %s <ip addr>\n", argv[0]);
		return 1;
	}

	ip_addr = argv[1];

	printf("Creating socket ... ");
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0 /* IPPROTO_TCP */)) == -1)
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
	my_addr.sin_addr.s_addr = inet_addr(ip_addr);
	bzero(&(my_addr.sin_zero), 8);

	printf("Binding socket to %s ... ", ip_addr);
	if (bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
		perror("bind");
		close(sock_fd);
		return 4;
	}
	printf("done\n");

	printf("Listening on socket ... ");
	if (listen(sock_fd, Q_LEN) == -1)
	{
		perror("listen");
		close(sock_fd);
		return 5;
	}
	printf("done\n");

	sin_size = sizeof(their_addr);
	printf("Waiting for accepting connections on socket ...\n");
	if ((eth_fd = accept(sock_fd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
	{
		perror("accept");
		close(sock_fd);
		return 6;
	}
	printf("Received a connection from %s. Processing its data ...\n", inet_ntoa(their_addr.sin_addr));
	process_eth(eth_fd, inet_ntoa(their_addr.sin_addr));
	printf("Done\n");

	close(eth_fd);
	close(sock_fd);

	return 0;
}
