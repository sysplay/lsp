/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney, 25/11/94
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

int main(int argc, char *argv[])
{
	struct sockaddr_in addr;
	int fd, cnt;
	struct ip_mreq mreq;
	char *message = "Hello, World!";
	int ttl = 64;

	/* Create what looks like an ordinary UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

	/* Set up destination address */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
	addr.sin_port = htons(HELLO_PORT);

	/* Now just sendto() our destination! */
	printf("Multicasting \"%s\" ", message);
	while (1)
	{
		if (sendto(fd, message, strlen(message) + 1, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			perror("sendto");
			exit(1);
		}
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	printf("Done\n");
	return 0;
}
