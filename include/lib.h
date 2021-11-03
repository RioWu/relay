#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
// wrapped functions
void Bind(int fd, const sockaddr *addr, socklen_t len);
void Connect(int fd, const sockaddr *addr, socklen_t len);
void Listen(int fd, int n);
int Socket(int domain, int type, int protocol);
int Accept(int fd, sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len);
int Fcntl(int fd, int cmd, int arg);
void writeN(int fd, char *buf, int n);
void readN(int fd, char *buf, int n);
