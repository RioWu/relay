#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include "client_map.h"
#define EpollEvents 128
class CLClientEpoll
{
private:
    int epoll_fd;
    int num_session_finished;
    int num_session_failed;
    std::string str;
    int string_length;
    clock_t start_time;
    epoll_event *events;

public:
    CLClientMap client_map;
    CLClientEpoll(std::string str, int string_length);
    ~CLClientEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doRead(int socket_fd);
    void doWrite(int socket_fd);
    void addEvent(int fd, int option);
    void modifyEvent(int fd, int option);
    void setStartTime(clock_t time);
};