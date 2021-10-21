#include "server_epoll.h"
#include "lib.h"
#include "fcntl.h"
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
    addBindPair(client_id, conn_fd);
    // set nonblock mode
    Fcntl(conn_fd, F_SETFL, Fcntl(conn_fd, F_GETFL, 0) | O_NONBLOCK);
    // after a pair of socket connect successfully,add event;
    // odd client should read first
    if (client_id % 2 == 0)
    {
        int conn_fd_odd = getConnFd(client_id - 1);
        addEvent(conn_fd_odd, 1);
    }
}
void CLServerEpoll::doRead(int conn_fd)
{
    int client_id = getClientId(conn_fd);
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

        addData(client_id - 1, data);
        int odd_conn_fd = getConnFd(client_id - 1);
        addEvent(odd_conn_fd, 0);
    }
    // after odd conn read,even conn write
    else if (client_id % 2 == 1)
    {
        addData(client_id + 1, data);
        int even_conn_fd = getConnFd(client_id + 1);
        addEvent(even_conn_fd, 0);
    }
    deleteEvent(conn_fd);
}
void CLServerEpoll::doWrite(int conn_fd)
{
    // after conn write,the same conn read
    int client_id = getClientId(conn_fd);
    char buf[string_length];
    std::string str = getData(client_id);
    strcpy(buf, str.c_str());
    writeN(conn_fd, buf, string_length);
    deleteEvent(conn_fd);
    addEvent(conn_fd, 1);
    deleteData(client_id);
}
// option = 0:add writable event
// option = 1:add readable event
void CLServerEpoll::addEvent(int fd, int option)
{
    if (option == 0)
    {
        struct epoll_event event_writable;
        event_writable.data.fd = fd;
        event_writable.events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_writable);
    }
    if (option == 1)
    {
        struct epoll_event event_readable;
        event_readable.data.fd = fd;
        event_readable.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_readable);
    }
}
void CLServerEpoll::deleteEvent(int socket_fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL);
}

void CLServerEpoll::addBindPair(int client_id, int conn_fd)
{
    client_conn_map.insert(pair<int, int>(client_id, conn_fd));
    conn_client_map.insert(pair<int, int>(conn_fd, client_id));
}
int CLServerEpoll::getClientId(int conn_fd)
{
    if (conn_client_map.find(conn_fd) == conn_client_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return conn_client_map.at(conn_fd);
}
int CLServerEpoll::getConnFd(int client_fd)
{
    if (client_conn_map.find(client_fd) == client_conn_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return client_conn_map.at(client_fd);
}
void CLServerEpoll::addData(int client_id, std::string data)
{
    std::pair<unordered_map<int, std::string>::iterator, bool> ret;
    ret = data_map.insert(pair<int, std::string>(client_id, data));
    if (ret.second == false)
    {
        printf("add data to data_map has failed\n");
        exit(0);
    }
}
std::string CLServerEpoll::getData(int client_id)
{
    if (data_map.find(client_id) == data_map.end())
    {
        printf("element not find\n");
        exit(0);
    }
    else
        return data_map.at(client_id);
}
void CLServerEpoll::deleteData(int client_id)
{
    data_map.erase(client_id);
}