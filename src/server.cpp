#include "server_epoll.h"
int main(int argc, const char *argv[])
{
    int listen_fd;
    struct sockaddr_in serv_addr;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(6666);
    bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listen_fd, LISTENQ);
    CLServerEpoll server_epoll(listen_fd);
    server_epoll.work();
}