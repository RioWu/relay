#include "client_epoll.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        perror("usage:client session_num string_length");
        exit(0);
    }

    int session_num = atoi(argv[1]);
    int string_length = atoi(argv[2]);
    int sockfd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    CLClientEpoll client_epoll(string_length);
    for (int i = 1; i < session_num * 2; i++)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        client_epoll.addEvent(sockfd, 0);
        client_epoll.addEvent(sockfd, 1);
    }
    client_epoll.work();
}