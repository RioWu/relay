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
  
- 2021.10.18

  - 记录一下遇到的一个`core dumped`的bug：在给`epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout)`中的参数`events`分配内存空间时，分配的空间大小一定要和参数`maxevents`对应，如果`maxevents`大小大于`events`实际的空间大小，就会产生内存溢出。就本程序而言，内存溢出导致了对`unordered_map`结构的破坏，并因此在后续对`unordered_map`进行操作时产生了`core dumped`。

  - 记录一个关于`char *`到`std::string`转化的问题，在程序中我设计了一个逻辑用于判断中继服务的正确性：当奇数客户端读数据时（这代表数据已经经历了两次中继：从奇数客户端到偶数客户端，再从偶数客户端到奇数客户端），比较接收到的数据和一开始发送的数据，如果相同，则表示中继服务正常运行，反之则表示有错误发生。但在运行中会出现这样一种情况：显示中继服务出现错误，但打印出来的接收到的数据和与之比较的正确数据是一样的，只是接收到的数据的长度比正确数据多1，通过增加下列语句可以解决这个问题：

    ```c++
    if ((int)received_data.length() == string_length + 1)
    	received_data.pop_back();
    ```

  - 问题产生的原因可能是在程序运行中，有多次的`char *`类型和`std::string`类型之间的类型转换（因为中继的数据使用`std::string`存储的，但在通过socket发送数据时又需要将string转化成char*，这其中可能产生了预期之外的问题，比如这个结束符`\0`。

  - todo:

    - 继续研究上面那个`char *`到`std::string`转化的问题
    - 解决bug：当传输数据较大时（10KB），服务端程序无法正常运行，数据无法正常添加至map中
      - 可能是数据过大，通过socket传输时出现bug。
      - 探究下为什么在addData时没有报错，直到getData时才显示`element not find`。
  
- 2021.10.19

  - 在测试时发现，如果将报文的长度增加，之前提到的接收到数据长度的问题又有新的变化，如果数据长度增加到1000，那么接收到的数据长度会变成1002，原因未知。![image-20211018173846934](/home/riochen/.config/Typora/typora-user-images/image-20211018173846934.png)

  - 通过以下操作可以简单的解决这个问题：

    ```c++
    while ((int)received_data.length() > string_length)
    {
    	received_data.pop_back();
    }
    ```

- 2021.10.20

  - 测试时发现，如果将报文长度选取至任务书所要求的最大长度（即10000字节），服务端会出现bug：奇数连接接受数据后会触发一次偶数连接的写事件，以将数据中继给偶数客户端，这之后本应该触发的是偶数连接的写事件，但程序运行时又会重复触发写事件，按照程序的逻辑，每次写事件触发完，都会删除对应事件，并且删除暂存的数据，这就导致在重复触发写事件后 ，因为找不到暂存的数据而报错`element not find`。

  - 经过调试，发现服务端接受到的数据长度为10005，在服务端也添加了如下代码用于去除多余字节，程序于是正常运行，然而原因仍然未知

    ```c++
    while ((int)data.length() > string_length)
    {
    	data.pop_back();
    }
    ```

- 2021.10.21

  - 更改压力发生器（客户端）发送报文逻辑：在用户主动停止之前，发生器会一直进行数据的发送
  - todo：
    - 学习测试工具的使用，对程序进行测试
      - nomn：系统性能分析工具：
        1. 查看cpu usr和sys在程序运行中的占比并思考状态是否符合预期
        2. 看网络带宽
      - netstat：网络状态分析工具：
        1. 主要查看连接状态和连接数（例子：当客户端连接数多了的时候检查是否全部都连上了，`netstat -ant|grep 6666|grep ESTABLISHED|wc -l`）
        2. 查看分析发送接收缓冲区的数据堆积状态，若有堆积，思考其原因。
      - valgrind：内存检查工具
      - gprof：程序代码性能分析工具
        - 主要查看函数执行时间和次数，思考那些函数在运行的时间占比高，并考虑能否对其优化。（对比：只设置编译参数-pg；设置编译参数-pg -no-pie -static;两者对于gprof的输出的区别） 
      - tcptrack:TCP连接跟踪
        - 监视来自指定IP和网卡的流量，对比思考tcptrack中统计的总带宽，和nmon中网卡带宽的区别
      - tcpdump：网络协议抓包工具