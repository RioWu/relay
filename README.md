# relay_server

- 和客户端建立连接，并在建立连接时创建会话（一个会话包含两个客户端），接收客户端传来的数据，并将数据转发至会话另一端。

### CLServerMap

管理服务端用到的map

- `std::unordered_map<int, bool> available_map`

  - 存储conn_fd的可用信息

- `    std::unordered_map<int, int> session_map;`

  - 存储conn_fd的配对信息

- `    std::unordered_map<int, std::string> data_map;`

  - 存储要转发的数据

- ```c++
  bool isAvailable(int conn_fd);
  void addAvailablePair(int conn_fd, bool status);
  void modifyAvailablePair(int conn_fd, bool staus);
  
  void addSession(int key_conn_fd, int value_conn_fd);
  int searchSessionPair(int key_conn_fd);
  
  void addData(int value_conn_fd, std::string data);
  std::string getData(int value_conn_fd);
  ```

### CLServerEpoll

使用epoll进行程序逻辑处理

```c++
CLServerEpoll(int listen_fd);
~CLServerEpoll();
void work();
void handleEpoll(epoll_event *events, int num_events);
void doAccept();
void doRead(int key_conn_fd);
void doWrite(int value_conn_fd);
void addEvent(int value_conn_fd, int option);
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
      void doRead(int conn_fd);
      void doWrite(int conn_fd);
      void addEvent(int conn_fd, int option);
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


2021.10.12

- version 1.1
- 内容：更改会话配对的逻辑
  - 原本的逻辑是：客户端发起连接，服务端在使用`accept`建立连接的同时，通过维护变量`n_unmapped_conn（该变量的取值只能为0，1，2）`来将两个紧跟着的建立的连接进行配对（即相应的`conn_fd`进行配对），然后每个客户端各自生成指定长度的随机字符串，发送到服务器进行中继。这个逻辑的问题是：客户端不知道自己的配对情况，也不知道自己应该要收到什么样的数据，无法对接收情况的正确与否进行判别。
  - 更新后的逻辑：客户端在建立连接时创建一个ID（从1开始直到10000），并在执行`connect`的时候发送给服务器，服务器端于是将`conn_fd`和对应的`ID`进行绑定，客户端和服务端都遵循以下的约定进行会话绑定：从1开始，相邻的两个ID进行绑定，奇数ID的客户端先发送数据，对应的偶数ID的客户端接收数据之后再将数据发送回奇数ID的客户端。
  - 客户端建立一个存放每个会话转发数据的map，用于让客户端对接收到的数据进行判断。
  - 会话ID和奇数ID的对应关系为：`会话ID = （奇数ID+1）/2`。
    - 例子：由`ID=19`和`ID=20`的客户端所组成的会话的ID为`(19+1)/2=10`。
- todo:
  - 不用每个会话都要生成不一样的字符串，程序的目的是测试高并发情况下系统的负荷情况，对所有会话生成同样的随即字符串即可。