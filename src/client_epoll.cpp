#include "client_epoll.h"
CLClientEpoll::CLClientEpoll(int length)
{
    CLClientEpoll::length = length;
    num_session_finished = 0;
    num_session_failed = 0;
    epoll_fd = epoll_create1(0);
    events = (epoll_event *)calloc(EpollEvents, sizeof(epoll_event));
}
CLClientEpoll::~CLClientEpoll()
{
    close(epoll_fd);
    delete[] events;
}
std::string CLClientEpoll::generateString()
{
    char tmp;
    std::string str;

    std::random_device rd;
    std::default_random_engine random(rd());

    for (int i = 0; i < length; i++)
    {
        // 10numbers plus 26 characters equals 36
        tmp = random() % 36;
        if (tmp < 10)
        {
            // tmp becomes a number
            tmp += '0';
        }
        else
        {
            // tmp becomes a character
            tmp -= 10;
            tmp += 'A';
        }
        str += tmp;
    }
    return str;
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
        int fd = events[i].data.fd;
        if (events[i].events & EPOLLIN)
            doRead(fd);
        else if (events[i].events & EPOLLOUT)
            doWrite(fd);
    }
}
void CLClientEpoll::doRead(int connfd)
{
    int num_read;
    char buf[MaxSize];
    num_read = read(connfd, buf, MaxSize);
}
void CLClientEpoll::doWrite(int connfd)
{
    std::string str = generateString();
    int num_write;
    num_write = write(connfd, (char *)str.data(), MaxSize);
    addEvent(connfd, 0);
}
//option = 0:add writable event
//option = 1:add readable event
void CLClientEpoll::addEvent(int connfd, int option)
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