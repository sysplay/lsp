/* This file contains the Socket Layer Wrapper APIs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define BACKLOG 1
#define BUF_SIZE 256

int get_mac_addr(const char *iface, char *macaddr)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *saddr;

	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return -1;
	}
	strcpy(ifr.ifr_name, iface);
	if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1)
	{
		perror("ioctl ifaddr");
		close(fd);
		return -1;
	}
	saddr = (struct sockaddr_in *)(&ifr.ifr_addr);
	strcpy(macaddr, inet_ntoa(saddr->sin_addr));
	close(fd);

	return 0;
}

int get_ip_addr(const char *iface, char *ipaddr, char *nmask)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *saddr;

	if ((fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return -1;
	}
	strcpy(ifr.ifr_name, iface);
	if (ioctl(fd, SIOCGIFADDR, &ifr) == -1)
	{
		perror("ioctl ifaddr");
		close(fd);
		return -1;
	}
	saddr = (struct sockaddr_in *)(&ifr.ifr_addr);
	strcpy(ipaddr, inet_ntoa(saddr->sin_addr));
	if (nmask)
	{
		strcpy(ifr.ifr_name, iface);
		if (ioctl(fd, SIOCGIFNETMASK, &ifr) == -1)
		{
			perror("ioctl ifnm");
			close(fd);
			return -1;
		}
		saddr = (struct sockaddr_in *)(&ifr.ifr_netmask);
		strcpy(nmask, inet_ntoa(saddr->sin_addr));
	}
	close(fd);

	return 0;
}

int open_socket(const char *ip, int port)
{
	int fd;
	int yes;
	struct sockaddr_in my_addr;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return -1;
	}

	/* Get rid of "address already in use" error message */
	yes = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		return -1;
	}

	my_addr.sin_family = AF_INET; // address family 
	my_addr.sin_port = htons(port); // short, network byte order
	my_addr.sin_addr.s_addr = inet_addr(ip);
	bzero(&(my_addr.sin_zero), 8);
 
	if (bind(fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		close(fd);
		return -1;
	}
    	
	if (listen(fd, BACKLOG) == -1)
	{
		perror("listen");
		close(fd);
		return -1;
	}

	return fd;
}

void close_socket(int sfd)
{
	close(sfd);
}

int get_socket(int sfd, char *client_ip)
{
	int fd;
	socklen_t sin_size;
	struct sockaddr_in their_addr;

	sin_size = sizeof(struct sockaddr_in);
	if((fd = accept(sfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
	{
		perror("accept");
		return -1;
	}
	if (client_ip)
	{
		strcpy(client_ip, inet_ntoa(their_addr.sin_addr));
	}

	return fd;
}

void put_socket(int fd)
{
	close(fd);
}

int printf_eth(int fd, const char *fmt, ...)
{
	va_list ap;
	char buf[BUF_SIZE], tbuf[BUF_SIZE];
	int print_cnt, ret, send_cnt;
	int i, j;

	va_start(ap, fmt);
	print_cnt = vsnprintf(buf, BUF_SIZE, fmt, ap);
	va_end(ap);
	if (print_cnt >= BUF_SIZE)
	{
		printf("Network printf string bigger than the base buffer size\n");
		return 0;
	}
	send_cnt = 0;
	for (i = 0, j = 0; i < print_cnt; i++)
	{
		tbuf[j] = buf[i];
		if (tbuf[j] == '\n')
		{
			tbuf[j++] = '\r';
			tbuf[j++] = '\n';
			ret = send(fd, tbuf, j, 0);
			j = 0;
			if (ret == -1)
			{
				perror("send");
				send_cnt = -1;
				break;
			}
			else
			{
				send_cnt += ret;
			}
		}
		else
		{
			j++;
		}
	}
	if (j != 0)
	{
		if ((ret = send(fd, tbuf, j, 0)) == -1)
		{
			perror("send");
			send_cnt = -1;
		}
		else
		{
			send_cnt += ret;
		}
	}
	return send_cnt;
}

int scanf_eth(int fd, const char *fmt, ...)
{
	char buf[BUF_SIZE];
	int ret_val, scan_cnt;
	va_list ap;

	scan_cnt = 0;
	do
	{
		ret_val = recv(fd, buf + scan_cnt, BUF_SIZE - scan_cnt, 0);
		scan_cnt += ret_val;
	}
	while ((ret_val > 0) && (buf[scan_cnt - 1] != '\n'));
	if (ret_val <= 0)
	{
		if (!ret_val)
		{
			errno = 0;
		}
		perror("recv");
		return -1;
	}
	else
	{
		buf[scan_cnt] = 0;
	}
	if (scan_cnt >= BUF_SIZE)
	{
		printf("Network scanf string bigger than the base buffer size\n");
		return 0;
	}
	va_start(ap, fmt);
	scan_cnt = vsscanf(buf, fmt, ap);
	va_end(ap);
	return scan_cnt;
}

int write_eth(int fd, const void *buf, unsigned int size)
{
	int write_cnt;

	if ((write_cnt = send(fd, buf, size, 0)) == -1)
	{
		perror("send");
	}
	return write_cnt;
}

int read_eth(int fd, void *buf, unsigned int size)
{
	int read_cnt;

	if ((read_cnt = recv(fd, buf, size, 0)) <= 0)
	{
		if (!read_cnt)
		{
			errno = 0;
		}
		perror("recv");
		return -1;
	}
	return read_cnt;
}
