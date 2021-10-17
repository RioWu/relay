# 版本记录

2021.10.10 

- 内容：实现报文转发功能

- todo：

  - 错误处理

  - 进行压力测试及结果分析

  - 使用非阻塞I/O

2021.10.12

- 内容：更改会话配对的逻辑
  - 原本的逻辑是：客户端发起连接，服务端在使用`accept`建立连接的同时，通过维护变量`n_unmapped_conn（该变量的取值只能为0，1，2）`来将两个紧跟着的建立的连接进行配对（即相应的`conn_fd`进行配对），然后每个客户端各自生成指定长度的随机字符串，发送到服务器进行中继。这个逻辑的问题是：客户端不知道自己的配对情况，也不知道自己应该要收到什么样的数据，无法对接收情况的正确与否进行判别。
  - 更新后的逻辑：客户端在建立连接时创建一个ID（从1开始直到10000），并在执行`connect`的时候发送给服务器，服务器端于是将`conn_fd`和对应的`ID`进行绑定，客户端和服务端都遵循以下的约定进行会话绑定：从1开始，相邻的两个ID进行绑定，奇数ID的客户端先发送数据，对应的偶数ID的客户端接收数据之后再将数据发送回奇数ID的客户端。
  - 客户端建立一个存放每个会话转发数据的map，用于让客户端对接收到的数据进行判断。
  - 会话ID和奇数ID的对应关系为：`会话ID = （奇数ID+1）/2`。
    - 例子：由`ID=19`和`ID=20`的客户端所组成的会话的ID为`(19+1)/2=10`。
- todo:
  - 不用每个会话都要生成不一样的字符串，程序的目的是测试高并发情况下系统的负荷情况，对所有会话生成同样的随即字符串即可。

2021.10.14

- 内容：非阻塞版本

- debug记录：

  - [段错误](https://blog.csdn.net/weixin_40877924/article/details/108762118)如何调试

  - 如何使用socket去接受和发送一个int(**大小端转化**)

    ```c++
    发：
    int a = 10;
    a = htonl(a);
    
    send(..., (const void*)&a, sizeof(a), ...);
    
    
    收：
    int a;
    recv(..., (void*)&a, sizeof(a),...);
    
    printf("%d\n", ntohl(a));
    ```

    - 思考：只要传输超过一个字节的类型的数据，都需要进行字节序的转化
  
  - `read`和`write`函数的`size`参数需要谨慎选择，否则可能会因为读取多余数据导致bug
  
  - 关于`epoll_wait`中的参数`int maxevents`和`epoll_event *events`：
  
    - At any single call of epoll_wait you'll at most receive as many events as you have room for, but of course events don't get lost if there are more than that queued up -- you'll simply get them at a later call. Since you'll be calling epoll_wait in a loop anyway, that shouldn't be an issue at all.
  
- 2021.10.17

  - 使用ET模式+非阻塞时，遇到非常奇怪的问题：客户端1转发数据至配对的客户端2，客户端2接受数据，到这里程序一切正常，但之后客户端2始终无法触发EPOLLOUT事件，无法将数据再转发给客户端1。
  - 将ET模式改为LT模式，并在每次触发事件后删除对应事件（以免不必要的重复触发多次事件，尤其是写事件），程序正常运行，但仍然不确定使用ET模式出现BUG的原因。