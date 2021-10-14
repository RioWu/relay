#include "lib.h"
void Bind(int fd, const sockaddr *addr, socklen_t len)
{
    int ret = bind(fd, addr, len);
    if (ret < 0)
    {
        perror("bind failed");
        exit(0);
    }
}
void Connect(int fd, const sockaddr *addr, socklen_t len)
{
    int ret = connect(fd, addr, len);
    if (ret < 0)
    {
        perror("connect failed");
        exit(0);
    }
}
void Listen(int fd, int n)
{
    int ret = listen(fd, n);
    if (ret < 0)
    {
        perror("listen failed");
        exit(0);
    }
}
int Socket(int domain, int type, int protocol)
{
    int ret = socket(domain, type, protocol);
    if (ret < 0)
    {
        perror("create socket failed");
        exit(0);
    }
    return ret;
}
int Accept(int fd, sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len)
{
    int ret = accept(fd, addr, addr_len);
    if (ret < 0)
    {
        perror("accept failed");
        exit(0);
    }
    return ret;
}
int Fcntl(int fd, int cmd, int arg)
{
    int ret = fcntl(fd, cmd, arg);
    if (ret < 0)
    {
        perror("fcntl failed");
        exit(0);
    }
    return ret;
}
void writeN(int fd, char *buf, int n, int length)
{
    int n_left = length;
    int write_pointer = 0;
    while (n_left > 0)
    {
        int n_write = 0;
        n_write = write(fd, buf + write_pointer, n);
        if (errno == EAGAIN)
            n_write = 0;
        else
        {
            n_left = n_left - n_write;
            write_pointer = write_pointer + n_write;
        }
    }
}
void keepRead(int fd, char *buf, int n)
{

    int read_pointer = 0;
    while (errno != EAGAIN)
    {
        int n_read = 0;
        n_read = read(fd, buf + read_pointer, n);
        read_pointer = read_pointer + n_read;
    }
}