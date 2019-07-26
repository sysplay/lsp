/* This header lists all the Socket Layer interfacing API's */
#ifndef SOCKET_LAYER_H
#define SOCKET_LAYER_H

int get_mac_addr(const char *iface, char *macaddr);
int get_ip_addr(const char *iface, char *ipaddr, char *nmask);
int open_socket(const char *ip, int port);
void close_socket(int sfd);
int get_socket(int sfd, char *client_ip);
void put_socket(int fd);
int printf_eth(int fd, const char *fmt, ...);
int scanf_eth(int fd, const char *fmt, ...);
int write_eth(int fd, const void *buf, unsigned int size);
int read_eth(int fd, void *buf, unsigned int size);

#endif
