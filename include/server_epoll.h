#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server_map.h"
#define EpollEvents 10000
#define MaxSize 10240
#define LISTENQ 1024
class CLServerEpoll
{
private:
    int epoll_fd;
    int listen_fd;
    CLServerMap server_map;
    epoll_event *events;

public:
    CLServerEpoll(int listen_fd);
    ~CLServerEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doAccept();
    void doRead(int conn_fd);
    void doWrite(int conn_fd);
    void addEvent(int fd, int option);
};