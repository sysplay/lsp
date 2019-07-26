//#define CLEANUP_ZOMBIE

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#ifdef CLEANUP_ZOMBIE
#include <signal.h>
#endif

#define PORT_NUM 2000
#define MAX_CONNECTIONS 10
#define Q_LEN MAX_CONNECTIONS
#define BUF_SIZE 100

#ifdef CLEANUP_ZOMBIE
static void cleanup_zombie(int sno)
{
	int child_status;

	wait(&child_status);
}
#endif

static void serve_the_eth(int efd, char *from_ip)
{
	int read_cnt;
	char buf[BUF_SIZE];

	while (1)
	{
		if ((read_cnt = recv(efd, buf, BUF_SIZE, 0)) <= 0)
		{
			printf("CS: Connection from %s went down\n", from_ip);
			close(efd);
			break;
		}
		buf[read_cnt - 1] = 0;
		printf("CS: %s: %s\n", from_ip, buf);
	}
}

int main(int argc, char *argv[])
{
	int sock_fd;
	int yes;
	char *ip_addr;
	struct sockaddr_in my_addr;

	socklen_t sin_size;
	struct sockaddr_in their_addr;
	int efd;
	pid_t child_pid;

#ifdef CLEANUP_ZOMBIE
	struct sigaction sa;
#endif

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

#ifdef CLEANUP_ZOMBIE
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &cleanup_zombie;
	sigaction(SIGCHLD, &sa, NULL);
#endif

	while (1)
	{
		sin_size = sizeof(their_addr);
		printf("Waiting for accepting connections on socket ...\n");
		if ((efd = accept(sock_fd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
		{
			perror("accept");
		}
		else
		{
			printf("Received a connection from %s\n", inet_ntoa(their_addr.sin_addr));
			child_pid = fork();
			if (child_pid == 0) // Child
			{
				close(sock_fd); // Closing the well-known port
				printf("CS: Concurrent server (%d) started\n", getpid());
				serve_the_eth(efd, inet_ntoa(their_addr.sin_addr));
				printf("CS: Concurrent server (%d) exited\n", getpid());
				return 0;
			}
			else // Error or Parent 
			{
				if (child_pid < 0)
				{
					printf("Spawning concurrent server failed. Unable to serve\n");
				}
				close(efd); // Closing the ephemeral port
			}
		}
	}

	close(sock_fd);

	return 0;
}
