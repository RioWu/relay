#include "client_epoll.h"
#include "lib.h"
using namespace std;
CLClientEpoll::CLClientEpoll(std::string str, int string_length)
{
    CLClientEpoll::string_length = string_length;
    CLClientEpoll::str = str;
    num_session_finished = 0;
    num_session_failed = 0;
    epoll_fd = epoll_create1(0);
    events = (epoll_event *)calloc(128, sizeof(epoll_event));
}
CLClientEpoll::~CLClientEpoll()
{
    close(epoll_fd);
    delete[] events;
}
void CLClientEpoll::work()
{
    while (true)
    {
        int num_events;
        num_events = epoll_wait(epoll_fd, events, EpollEvents, -1);
        handleEpoll(events, num_events);
    }
}
void CLClientEpoll::handleEpoll(epoll_event *events, int num_events)
{
    for (int i = 0; i < num_events; i++)
    {
        int socket_fd = events[i].data.fd;
        if (events[i].events & EPOLLIN)
            doRead(socket_fd);
        else if (events[i].events & EPOLLOUT)
            doWrite(socket_fd);
    }
}
void CLClientEpoll::doRead(int socket_fd)
{
    char buf[string_length];
    int client_id = getClientId(socket_fd);
    // client_id is odd,after read,should verify the accuracy
    if (client_id % 2 == 1)
    {
        // in nonblock mode,should keep read until get EAGAIN error
        readN(socket_fd, buf, string_length);
        // char * to string
        std::string received_data(buf);
        if (received_data == str)
        {
            num_session_finished++;
            printf("has finished %d sessions\n", num_session_finished);
        }
        else
        {
            num_session_failed++;
            printf("has failed %d sessions", num_session_failed);
        }
        deleteEvent(socket_fd);
    }
    // client_id is even,after read,should write same data to matched client
    else if (client_id % 2 == 0)
    {
        readN(socket_fd, buf, string_length);
        // char * to string
        std::string received_data(buf);
        addData(client_id, received_data);
        deleteEvent(socket_fd);
        addEvent(socket_fd, 0);
    }
}
void CLClientEpoll::doWrite(int socket_fd)
{
    int client_id = getClientId(socket_fd);
    // client_id is odd
    if (client_id % 2 == 1)
    {
        int matched_socket = getSocketFd(client_id + 1);
        writeN(socket_fd, (char *)str.data(), string_length);
        addEvent(matched_socket, 1);
    }
    // client_id is even
    else if (client_id % 2 == 0)
    {
        int matched_socket = getSocketFd(client_id - 1);
        std::string str = getData(client_id);
        write(socket_fd, (char *)str.data(), string_length);
        addEvent(matched_socket, 1);
    }
    deleteEvent(socket_fd);
}
/**
 * option = 0:add writable event
 * option = 1:add readable event
 **/
void CLClientEpoll::addEvent(int socket_fd, int option)
{
    if (option == 0)
    {
        struct epoll_event event_writable;
        event_writable.data.fd = socket_fd;
        event_writable.events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event_writable);
    }
    if (option == 1)
    {
        struct epoll_event event_readable;
        event_readable.data.fd = socket_fd;
        event_readable.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event_readable);
    }
}
void CLClientEpoll::deleteEvent(int socket_fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL);
}

void CLClientEpoll::addData(int client_id, std::string data)
{
    data_map.insert(std::pair<int, std::string>(client_id, data));
}
std::string CLClientEpoll::getData(int client_id)
{
    if (data_map.find(client_id) == data_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return data_map.at(client_id);
}
void CLClientEpoll::addBindPair(int socket_fd, int client_id)
{
    sock_cli_map.insert(std::pair<int, int>(socket_fd, client_id));
    cli_sock_map.insert(std::pair<int, int>(client_id, socket_fd));
}
int CLClientEpoll::getClientId(int socket_fd)
{
    if (sock_cli_map.find(socket_fd) == sock_cli_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return sock_cli_map.at(socket_fd);
}
int CLClientEpoll::getSocketFd(int client_id)
{
    if (cli_sock_map.find(client_id) == cli_sock_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return cli_sock_map.at(client_id);
}