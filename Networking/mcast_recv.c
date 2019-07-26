/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *		the group to its stdout...
 *
 * Antony Courtney, 25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define HELLO_PORT 4000
#define HELLO_GROUP "225.0.0.37"
#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
	int yes;
	struct sockaddr_in addr;
	int fd, nbytes, addrlen;
	struct ip_mreq mreq;
	char msgbuf[MSGBUFSIZE];

	/* Create what looks like an ordinary UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	/* Allow multiple sockets to use the same PORT number */
	yes = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	/* Set up destination address */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B. differs from sender */
	addr.sin_port = htons(HELLO_PORT);

	/* Bind to receive address */
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(1);
	}

	/* Use setsockopt() to request that the kernel join a multicast group */
	mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	/* Now just enter a read-print loop */
	printf("Waiting for receiving multicast messages ...\n");
	while (1)
	{
		addrlen = sizeof(addr);
		if ((nbytes = recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen)) < 0)
		{
			perror("recvfrom");
			exit(1);
		}
		printf("%s: %s\n", inet_ntoa(addr.sin_addr), msgbuf);
	}
	printf("Done\n");
	return 0;
}
