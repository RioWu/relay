#include "client_epoll.h"
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include "lib.h"

std::string generateString(int length)
{
    char tmp;
    std::string str;

    std::random_device rd;
    std::default_random_engine random(rd());

    for (int i = 0; i < length; i++)
    {
        // 10numbers plus 26 characters equals 36
        tmp = random() % 36;
        if (tmp < 10)
        {
            // tmp becomes a number
            tmp += '0';
        }
        else
        {
            // tmp becomes a character
            tmp -= 10;
            tmp += 'A';
        }
        str += tmp;
    }
    return str;
}
int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        printf("usage:client session_num string_length\n");
        exit(0);
    }

    int session_num = atoi(argv[1]);
    int string_length = atoi(argv[2]);
    int socket_fd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    std::string str = generateString(string_length);
    printf("generated string is : %s\n", str.c_str());
    pause();
    CLClientEpoll client_epoll(str, string_length);
    for (int i = 1; i <= session_num * 2; i++)
    {
        int socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

        Connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        client_epoll.client_map.addBindPair(socket_fd, i, 0);
        client_epoll.client_map.addBindPair(socket_fd, i, 1);
        write(socket_fd, &i, MaxSize);
        // i is an even number,should write first
        if (i % 2 == 1)
            client_epoll.addEvent(socket_fd, 0);
        // i is an odd number,should read first
        if (i % 2 == 0)
            client_epoll.addEvent(socket_fd, 1);
    }
    //set nonblock mode
    int val = Fcntl(socket_fd, F_GETFL, 0);
    Fcntl(socket_fd, F_SETFL, val | O_NONBLOCK);
    client_epoll.work();
}