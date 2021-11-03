#include "server_epoll.h"
#include "lib.h"
#include "fcntl.h"
#include <string.h>
#include <string>
#include <sstream>
using namespace std;
CLServerEpoll::CLServerEpoll(int listen_fd, int string_length)
{
    CLServerEpoll::string_length = string_length;
    CLServerEpoll::listen_fd = listen_fd;
    epoll_fd = epoll_create1(0);
    events = (epoll_event *)calloc(128, sizeof(epoll_event));
}
CLServerEpoll::~CLServerEpoll()
{
    close(epoll_fd);
    delete[] events;
}
void CLServerEpoll::work()
{
    while (true)
    {
        int num_events;
        num_events = epoll_wait(epoll_fd, events, EpollEvents, -1);
        handleEpoll(events, num_events);
    }
}
void CLServerEpoll::handleEpoll(epoll_event *events, int num_events)
{
    for (int i = 0; i < num_events; i++)
    {
        int fd = events[i].data.fd;
        if ((fd == listen_fd) && (events[i].events & EPOLLIN))
            doAccept();
        else if (events[i].events & EPOLLIN)
            doRead(fd);
        else if (events[i].events & EPOLLOUT)
            doWrite(fd);
    }
}
void CLServerEpoll::doAccept()
{
    int client_id;
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int conn_fd = Accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    read(conn_fd, &client_id, sizeof(client_id));
    client_id = ntohl(client_id);
    server_map.addBindPair(client_id, conn_fd);
    // set nonblock mode
    Fcntl(conn_fd, F_SETFL, Fcntl(conn_fd, F_GETFL, 0) | O_NONBLOCK);
    // after a pair of socket connect successfully,add event;
    // odd client should read first
    if (client_id % 2 == 0)
    {
        int conn_fd_odd = server_map.getConnFd(client_id - 1);
        addEvent(conn_fd_odd, 1);
    }
}
void CLServerEpoll::doRead(int conn_fd)
{
    int client_id = server_map.getClientId(conn_fd);
    char buf[string_length];
    readN(conn_fd, buf, string_length);
    std::string data(buf);
    // sometimes data.length is larger than string_length
    while ((int)data.length() > string_length)
    {
        data.pop_back();
    }
    // after even conn read,odd conn write
    if (client_id % 2 == 0)
    {

        server_map.addData(client_id - 1, data);
        int odd_conn_fd = server_map.getConnFd(client_id - 1);
        addEvent(odd_conn_fd, 0);
    }
    // after odd conn read,even conn write
    else if (client_id % 2 == 1)
    {
        server_map.addData(client_id + 1, data);
        int even_conn_fd = server_map.getConnFd(client_id + 1);
        addEvent(even_conn_fd, 0);
    }
    deleteEvent(conn_fd);
}
void CLServerEpoll::doWrite(int conn_fd)
{
    // after conn write,the same conn read
    int client_id = server_map.getClientId(conn_fd);
    char buf[string_length];
    std::string str = server_map.getData(client_id);
    strcpy(buf, str.c_str());
    writeN(conn_fd, buf, string_length);
    deleteEvent(conn_fd);
    addEvent(conn_fd, 1);
    server_map.deleteData(client_id);
}
// option = 0:add writable event
// option = 1:add readable event
void CLServerEpoll::addEvent(int fd, int option)
{
    if (option == 0)
    {
        struct epoll_event event_writable;
        bzero(&event_writable, sizeof(event_writable));
        event_writable.data.fd = fd;
        event_writable.events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_writable);
    }
    if (option == 1)
    {
        struct epoll_event event_readable;
        bzero(&event_readable, sizeof(event_readable));
        event_readable.data.fd = fd;
        event_readable.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_readable);
    }
}
void CLServerEpoll::deleteEvent(int socket_fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL);
}
