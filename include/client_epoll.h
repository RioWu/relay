#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <random>
#define EpollEvents 1024
#define MaxSize 1024
class CLClientEpoll
{
private:
    int epoll_fd;
    int length;
    int num_session_finished;
    int num_session_failed;
    epoll_event *events;
    std::string generateString();

public:
    CLClientEpoll(int length);
    ~CLClientEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doRead(int connfd);
    void doWrite(int connfd);
    void addEvent(int connfd, int option);
};