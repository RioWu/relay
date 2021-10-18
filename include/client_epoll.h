#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <random>
#include <unordered_map>
#define EpollEvents 128
class CLClientEpoll
{
private:
    int epoll_fd;
    int num_session_finished;
    int num_session_failed;
    std::string str;
    int string_length;
    epoll_event *events;

    std::unordered_map<int, std::string> data_map;
    std::unordered_map<int, int> sock_cli_map;
    std::unordered_map<int, int> cli_sock_map;

public:
    CLClientEpoll(std::string str, int string_length);
    ~CLClientEpoll();
    void work();
    void handleEpoll(epoll_event *events, int num_events);
    void doRead(int socket_fd);
    void doWrite(int socket_fd);
    void addEvent(int fd, int option);
    void deleteEvent(int fd);

    void addData(int session_id, std::string data);
    std::string getData(int session_id);

    void addBindPair(int socket_fd, int client_id);
    int getClientId(int socket_fd);
    int getSocketFd(int clinet_id);
};