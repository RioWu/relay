#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <random>
#include "client_map.h"
#define EpollEvents 10240
class CLClientEpoll
{
private:
    int epoll_fd;
    int num_session_finished;
    int num_session_failed;
    std::string str;
    int string_length;
    epoll_event *events;

public:
    CLClientEpoll(std::string str, int string_length);
    ~CLClientEpoll();
    CLClientMap client_map;
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doRead(int socket_fd);
    void doWrite(int socket_fd);
    void addEvent(int fd, int option);
    void deleteEvent(int fd);
};