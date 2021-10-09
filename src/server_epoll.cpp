#include "server_epoll.h"
CLServerEpoll::CLServerEpoll(int listen_fd)
{
    n_unmapped_conn = 0;
    CLServerEpoll::listen_fd = listen_fd;
    current_stored_connfd = 0;
    epoll_fd = epoll_create1(0);
    events = (epoll_event *)calloc(EpollEvents, sizeof(epoll_event));
    addEvent(CLServerEpoll::listen_fd, 1);
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
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int connfd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    n_unmapped_conn++;
    if (n_unmapped_conn == 1)
    {
        current_stored_connfd = connfd;
    }
    else if (n_unmapped_conn == 2)
    {
        server_map.addSession(current_stored_connfd, connfd);
        server_map.addSession(connfd, current_stored_connfd);
        server_map.addAvailablePair(connfd, true);
        server_map.addAvailablePair(current_stored_connfd, true);
        addEvent(connfd, 1);
        addEvent(current_stored_connfd, 1);
        n_unmapped_conn = 0;
    }
}
void CLServerEpoll::doRead(int key_connfd)
{
    int value_connfd = server_map.searchSessionPair(key_connfd);
    int num_read;
    char buf[MaxSize];
    num_read = read(key_connfd, buf, MaxSize);
    if (num_read == -1 || num_read == 0)
    {
        server_map.modifyAvailablePair(key_connfd, false);
    }
    else
    {
        //char* type to string type
        std::string str = buf;
        server_map.addData(value_connfd, str);
        addEvent(value_connfd, 0);
    }
}
void CLServerEpoll::doWrite(int value_connfd)
{
    int num_write;
    std::string data = server_map.getData(value_connfd);
    //string type to char* type
    num_write = write(value_connfd, (char *)data.data(), MaxSize);
    if (num_write == -1)
    {
        server_map.modifyAvailablePair(value_connfd, false);
    }
}
//option = 0:add writable event
//option = 1:add readable event
void CLServerEpoll::addEvent(int connfd, int option)
{
    if (option == 0)
    {
        struct epoll_event event_writable;
        event_writable.data.fd = connfd;
        event_writable.events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event_writable);
    }
    if (option == 1)
    {
        struct epoll_event event_readable;
        event_readable.data.fd = connfd;
        event_readable.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event_readable);
    }
}