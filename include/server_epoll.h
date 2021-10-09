#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server_map.h"
#define EpollEvents 1024
#define MaxSize 1024
#define LISTENQ 1024
class CLServerEpoll
{
private:
    int epoll_fd;
    int listen_fd;
    int n_unmapped_conn;
    int current_stored_connfd;
    CLServerMap server_map;
    epoll_event *events;

public:
    CLServerEpoll(int listen_fd);
    ~CLServerEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doAccept();
    void doRead(int key_connfd);
    void doWrite(int value_connfd);
    void addEvent(int value_connfd, int option);
};