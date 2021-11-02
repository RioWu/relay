#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unordered_map>
#include "server_map.h"
#define EpollEvents 128
#define LISTENQ 10
class CLServerEpoll
{
private:
    int epoll_fd;
    int listen_fd;
    int string_length;
    epoll_event *events;

public:
    CLServerMap server_map;
    CLServerEpoll(int listen_fd, int string_length);
    ~CLServerEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doAccept();
    void doRead(int conn_fd);
    void doWrite(int conn_fd);
    void addEvent(int fd, int option);
    void deleteEvent(int fd);
};
