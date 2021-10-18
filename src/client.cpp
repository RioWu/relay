#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <string.h>
#include "lib.h"
#include "client_epoll.h"

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
    int client_id;
    int session_num = atoi(argv[1]);
    int string_length = atoi(argv[2]);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    std::string str = generateString(string_length);
    // printf("generated string is : %s\n", str.c_str());
    CLClientEpoll client_epoll(str, string_length);
    int socket_init = Socket(AF_INET, SOCK_STREAM, 0);
    Connect(socket_init, (struct sockaddr *)&servaddr, sizeof(servaddr));
    string_length = htonl(string_length);
    write(socket_init, &string_length, sizeof(string_length));
    string_length = ntohl(string_length);
    for (client_id = 1; client_id <= session_num * 2; client_id++)
    {
        int socket_fd = Socket(AF_INET, SOCK_STREAM, 0);
        // connect use block mode
        Connect(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        client_epoll.addBindPair(socket_fd, client_id);

        client_id = htonl(client_id);
        // first write use block mode
        write(socket_fd, &client_id, sizeof(client_id));
        client_id = ntohl(client_id);
        // set nonblock mode
        Fcntl(socket_fd, F_SETFL, Fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK);

        // after a pair of socket connect successfully,add event;
        // odd client should write first
        if (client_id % 2 % 2 == 0)
        {
            int socket_fd_odd = client_epoll.getSocketFd(client_id - 1);
            client_epoll.addEvent(socket_fd_odd, 0);
        }
    }

    client_epoll.work();
}