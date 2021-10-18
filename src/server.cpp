#include "server_epoll.h"
#include "lib.h"
#include "fcntl.h"
int main(int argc, const char *argv[])
{
    int listen_fd;
    int string_length;
    struct sockaddr_in serv_addr;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(6666);
    Bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(listen_fd, LISTENQ);
    // init,first receive the string_length
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int conn_fd = Accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    read(conn_fd, &string_length, sizeof(string_length));
    string_length = ntohl(string_length);
    CLServerEpoll server_epoll(listen_fd, string_length);
    server_epoll.addEvent(listen_fd, 1);
    server_epoll.work();
}