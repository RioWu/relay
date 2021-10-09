# relay_server

- 和客户端建立连接，并在建立连接时创建会话（一个会话包含两个客户端），接收客户端传来的数据，并将数据转发至会话另一端。

### CLServerMap

管理服务端用到的map

- `std::unordered_map<int, bool> available_map`

  - 存储connfd的可用信息

- `    std::unordered_map<int, int> session_map;`

  - 存储connfd的配对信息

- `    std::unordered_map<int, std::string> data_map;`

  - 存储要转发的数据

- ```c++
  bool isAvailable(int connfd);
  void addAvailablePair(int connfd, bool status);
  void modifyAvailablePair(int connfd, bool staus);
  
  void addSession(int key_connfd, int value_connfd);
  int searchSessionPair(int key_connfd);
  
  void addData(int value_connfd, std::string data);
  std::string getData(int value_connfd);
  ```

### CLServerEpoll

使用epoll进行程序逻辑处理

```c++
CLServerEpoll(int listen_fd);
~CLServerEpoll();
void work();
void handleEpoll(epoll_event *events, int num_events);
void doAccept();
void doRead(int key_connfd);
void doWrite(int value_connfd);
void addEvent(int value_connfd, int option);
```

# relay_client

- 通过命令行参数输入要建立的会话数量以及要转发的报文大小，和服务器端建立连接，发送报文至服务器端，并由服务器端进行转发。

### CLClientEpoll

- ```c++
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
  ```

- 函数`generateString`用于根据用户输入的报文大小参数生成由随机字符和数字构成的字符串。



# 版本记录

- 2021.10.10 

  - version 1.0

  - 内容：实现报文转发功能

  - todo：

    - 错误处理

    - 进行压力测试及结果分析

    - 使用非阻塞I/O

      