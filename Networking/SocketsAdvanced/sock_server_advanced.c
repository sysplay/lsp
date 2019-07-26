/*
 * This is a multi-threaded server with multiple different sockets.
 * For clients, use telnet to connect as:
 *	telnet <server_ip> <socket_no from 1000 to 1006>
 * And then send characters, to be echoed back with a increment of
 * "<socket_no> - 1000"
 */
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <sys/select.h>

#include "socket_layer.h"

#define MAX_CONNECTIONS 7

static int eth_fd[MAX_CONNECTIONS] = {-1, -1, -1, -1, -1, -1, -1};
static char remote_ip[16];

static void *process_eth(void *arg)
{
	int i = (int)(long)(arg);
	char c;

	while (1)
	{
		if (read_eth(eth_fd[i], &c, 1) == -1)
		/* Error / Disconnect on eth */
		{
			printf("Eth fd %d down\n", eth_fd[i]);
			close(eth_fd[i]);
			eth_fd[i] = -1;
			return NULL;
		}

		/* Increment & Send Back */
		if ((c != '\n') && (c != '\r'))
		{
			printf("Received: %c; ", c);
			c += i;
		}

		if (write_eth(eth_fd[i], &c, 1) == -1)
		/* Error / Disconnect on eth */
		{
			printf("Eth fd %d down\n", eth_fd[i]);
			close(eth_fd[i]);
			eth_fd[i] = -1;
			return NULL;
		}
		if ((c != '\n') && (c != '\r'))
		{
			printf("Sent '%c' incremented by %d\n", c, i);
		}
	}
}

int main(int argc, char *argv[])
{
	char ip_addr[16];
	/*
	 * Multiple sockets are used for the user to connect simultaneously on
	 * different ports of same IP
	 */
	int soc_port[MAX_CONNECTIONS] = {1000, 1001, 1002, 1003, 1004, 1005, 1006};
	int sock_fd[MAX_CONNECTIONS];
	int n_fd, max_fd;
	fd_set input;
	pthread_t tid;
	int i, j;
	
	if (argc != 2)
	{
		printf("Usage : %s <nw i/f>\n", argv[0]);
		return 1;
	}
		
	get_ip_addr(argv[1], ip_addr, NULL);
	
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		if ((sock_fd[i] = open_socket(ip_addr, soc_port[i])) == -1)
		{
			for (j = (i - 1); j >= 0; j--)
			{
				close_socket(sock_fd[j]);
			}
			return 2;
		}
	}

	FD_ZERO(&input);
	max_fd = 0;
	for (i = 0; i < MAX_CONNECTIONS; i++)
	{
		FD_SET(sock_fd[i], &input);
		max_fd = (sock_fd[i] > max_fd) ? sock_fd[i] : max_fd;
	}
	max_fd += 1;

	while (1)
	{
		/* Do the select */
		n_fd = select(max_fd, &input, NULL, NULL, NULL);
		/* See if there was an error */
		if (n_fd < 0)
		{
			/* This program has been cancelled */
			break;
		}
		else
		{
			for (i = 0; i < MAX_CONNECTIONS; i++)
			{
				if ((eth_fd[i] == -1) && FD_ISSET(sock_fd[i], &input))
				/*
				 * Accept only one connection. Change this logic to accept more
				 * connections on the same port
				 */
				{
					if ((eth_fd[i] = get_socket(sock_fd[i], remote_ip)) != -1)
					{
						printf("Eth fd %d up\n", eth_fd[i]);
						if (pthread_create(&tid, NULL, process_eth, (void *)(long)(i)) != 0)
						{
							continue;
						}
						printf("Thread Id %lx created\n", tid);
					}
				}
			}
		}
		for (i = 0; i < MAX_CONNECTIONS; i++)
		{
			FD_SET(sock_fd[i], &input);
		}
	}

	for (i = (MAX_CONNECTIONS - 1); i >= 0; i--)
	{
		if (eth_fd[i] != -1)
		{
			close(eth_fd[i]);
		}
		close_socket(sock_fd[i]);
	}

	return 0;
}
