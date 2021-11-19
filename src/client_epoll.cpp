#include "client_epoll.h"
#include "lib.h"
#include <iostream>
#include <string.h>
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
    free(events);
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
    int client_id = client_map.getClientId(socket_fd);
    CLBuffer buffer = client_map.getBuffer(socket_fd);

    // client_id is odd,after read,should verify the accuracy
    if (client_id % 2 == 1)
    {
        int res = buffer.readByBuffer();
        if (res == 1)
        {
            buffer.getDataFromBuffer(buf);
            // char * to string
            std::string received_data = buf;
            while ((int)received_data.length() > string_length)
            {
                received_data.pop_back();
            }

            if (received_data == str)
            {
                num_session_finished++;
                // if (num_session_finished % 1000 == 0)
                printf("has finished %d sessions\n", num_session_finished);
                // 测试环境为本机，如果限制总转发数量，程序会结束过快而无法检测性能
                if (num_session_finished == 10000)
                {
                    clock_t end_time = clock();
                    std::cout << "run time is " << (double)(end_time - start_time) / CLOCKS_PER_SEC << std::endl;
                    printf("10000 times relay ok!\n");
                    // pause();
                }
                modifyEvent(socket_fd, 0);
            }
            else
            {
                num_session_failed++;
                printf("has failed %d sessions\n", num_session_failed);
                // printf("received data is %s,length is %lu\n", received_data.c_str(), received_data.length());
                // printf("the correct data is %s,length is %lu\n", str.c_str(), str.length());
                exit(0);
            }
        }
    }
    // client_id is even,after read,should write same data to matched client
    else if (client_id % 2 == 0)
    {
        int res = buffer.readByBuffer();
        if (res == 1)
        {
            buffer.getDataFromBuffer(buf);
            // char * to string
            std::string received_data = buf;
            client_map.addData(client_id, received_data);
            modifyEvent(socket_fd, 0);
        }
    }
}
void CLClientEpoll::doWrite(int socket_fd)
{
    int client_id = client_map.getClientId(socket_fd);
    CLBuffer buffer = client_map.getBuffer(socket_fd);
    // client_id is odd
    if (client_id % 2 == 1)
    {
        int res = buffer.writeByBuffer((char *)str.data());
        if (res == 1)
            modifyEvent(socket_fd, 1);
    }
    // client_id is even
    else if (client_id % 2 == 0)
    {
        std::string str = client_map.getData(client_id);
        int res = buffer.writeByBuffer((char *)str.data());
        if (res == 1)
            modifyEvent(socket_fd, 1);
    }
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
        bzero(&event_writable, sizeof(event_writable));
        event_writable.data.fd = socket_fd;
        event_writable.events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event_writable);
    }
    if (option == 1)
    {
        struct epoll_event event_readable;
        bzero(&event_readable, sizeof(event_readable));
        event_readable.data.fd = socket_fd;
        event_readable.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event_readable);
    }
}
/**
 * option = 0:modify to writable event
 * option = 1:modify to readable event
 **/
void CLClientEpoll::modifyEvent(int socket_fd, int option)
{
    if (option == 0)
    {
        struct epoll_event event_writable;
        bzero(&event_writable, sizeof(event_writable));
        event_writable.data.fd = socket_fd;
        event_writable.events = EPOLLOUT;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, socket_fd, &event_writable);
    }
    if (option == 1)
    {
        struct epoll_event event_readable;
        bzero(&event_readable, sizeof(event_readable));
        event_readable.data.fd = socket_fd;
        event_readable.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, socket_fd, &event_readable);
    }
}

void CLClientEpoll::setStartTime(clock_t time)
{
    CLClientEpoll::start_time = time;
}