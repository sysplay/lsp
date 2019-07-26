#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT_NUM 2000
#define MAX_CONNECTIONS 10
#define Q_LEN 1 // MAX_CONNECTIONS
#define FREE_FD 0
#define IS_FREE_FD(i) (eth_fd[i] == FREE_FD)
#define BUF_SIZE 100

static int eth_fd[MAX_CONNECTIONS];
static char ip_addr[MAX_CONNECTIONS][16];
static int current_connections;

static int find_eth_fd(int efd)
{
	int i;

	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (eth_fd[i] == efd)
		{
			break;
		}
	}
	return (i == MAX_CONNECTIONS) ? -1 : i;
}

static void setup_new_connection(int efd, char *from_ip)
{
	int free_i;

	if (current_connections == MAX_CONNECTIONS)
	{
		printf("Max connections (%d) reached\n", MAX_CONNECTIONS);
		close(efd);
	}
	else
	{
		printf("Received a connection from %s. Waiting for data ...\n", from_ip);
		free_i = find_eth_fd(FREE_FD);
		eth_fd[free_i] = efd;
		strcpy(ip_addr[free_i], from_ip);
		current_connections++;
	}
}

static void process_data_on_eth(int i)
{
	int read_cnt;
	char buf[BUF_SIZE];

	if ((read_cnt = recv(eth_fd[i], buf, BUF_SIZE, 0)) <= 0)
	{
		printf("Connection from %s went down\n", ip_addr[i]);
		close(eth_fd[i]);
		eth_fd[i] = FREE_FD;
		current_connections--;
	}
	else
	{
		buf[read_cnt - 1] = 0;
		printf("%s: %s\n", ip_addr[i], buf);
	}
}

int main(int argc, char *argv[])
{
	int sock_fd;
	int yes;
	char *ip_addr;
	struct sockaddr_in my_addr;

	int max_fd;
	fd_set fds;
	int i;

	socklen_t sin_size;
	struct sockaddr_in their_addr;
	int efd;

	if (argc != 2)
	{
		printf("Usage: %s <ip addr>\n", argv[0]);
		return 1;
	}

	ip_addr = argv[1];

	printf("Creating socket ... ");
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
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

	FD_ZERO(&fds);
	FD_SET(sock_fd, &fds);
	max_fd = sock_fd + 1;
	printf("Waiting for accepting connections on socket ...\n");
	while (select(max_fd, &fds, NULL, NULL, NULL) > 0)
	{
		if (FD_ISSET(sock_fd, &fds)) // New connection request
		{
			sin_size = sizeof(their_addr);
			if ((efd = accept(sock_fd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
			{
				perror("accept");
			}
			else
			{
				setup_new_connection(efd, inet_ntoa(their_addr.sin_addr));
			}
		}
		for (i = 0; i < MAX_CONNECTIONS; i++)
		{
			if (IS_FREE_FD(i))
			{
				continue;
			}
			if (FD_ISSET(eth_fd[i], &fds))
			{
				process_data_on_eth(i);
			}
		}

		FD_ZERO(&fds);
		FD_SET(sock_fd, &fds);
		max_fd = sock_fd;
		for (i = 0; i < MAX_CONNECTIONS; i++)
		{
			if (IS_FREE_FD(i))
			{
				continue;
			}
			FD_SET(eth_fd[i], &fds);
			if (eth_fd[i] > max_fd)
			{
				max_fd = eth_fd[i];
			}
		}
		max_fd += 1;
	}
	perror("select");

	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (!IS_FREE_FD(i))
		{
			close(eth_fd[i]);
		}
	}
	close(sock_fd);

	return 0;
}
