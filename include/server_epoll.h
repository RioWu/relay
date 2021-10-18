#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unordered_map>
#define EpollEvents 10240
#define LISTENQ 10
class CLServerEpoll
{
private:
    int epoll_fd;
    int listen_fd;
    int string_length;
    epoll_event *events;

    std::unordered_map<int, int> client_conn_map;
    std::unordered_map<int, int> conn_client_map;
    std::unordered_map<int, std::string> data_map;

public:
    CLServerEpoll(int listen_fd, int string_length);
    ~CLServerEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doAccept();
    void doRead(int conn_fd);
    void doWrite(int conn_fd);
    void addEvent(int fd, int option);
    void deleteEvent(int fd);

    void addBindPair(int client_id, int conn_fd);
    int getClientId(int conn_fd);
    int getConnFd(int client_id);
    void addData(int client_id, std::string data);
    void deleteData(int client_id);
    std::string getData(int client_id);
};
