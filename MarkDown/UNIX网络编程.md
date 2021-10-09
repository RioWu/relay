# 第1章 简介

## p5 intro/daytimetcpcli.c 时间获取程序（客户端）

-  `int socket(int domain, int type, int protocol);`
  -  用于建立网络通信，返回生成的socket文件描述符。
  -  参数domain指明通信所使用的协议族，这里AF_INET表示IPV4
  -  参数type指明socket的类型，这里SOCK_STREAM表示数据流套接字，即TCP
  -  参数protocol一般赋值为0

- ` void bzero(void *s, int n);`
  -  将内存块（字符串）的前n个字节清零，该函数位于头文件string.h中，但实际上也可以作用于其他类型

- ` htons()`
  - 将主机字节顺序（小尾顺序）转化为网络字节顺序（大尾顺序），即高低位互换。

- ` inet_pton()`
  - 将点分十进制类型的IP地址转化为二进制网络字节序的IP地址。

- ` int connect (int sockfd,struct sockaddr * serv_addr,int addrlen);`
  -  用于客户端建立tcp连接。
  -  sockfd:标识一个套接字。
  -  serv_addr：套接字s想要连接的主机地址和端口号。
  -  addrlen：缓冲区的长度

-  关于`struct sockaddr`和`struct sockaddr_in`：
  -  套接字编程需要指定套接字的地址作为参数，不同的协议族有不同的地址结构定义方式。这些地址结构通常以sockaddr_开头，每一个协议族有一个唯一的后缀，如IPV4的结构名称为sockaddr_in。
  -  sockaddr指的是通用的套接字地址类型，它可以在不同的协议族之间进行强制转换。

- ` int fputs(const char *s, FILE *stream);`
  -  将字符串s输出至流stream中

-  在主程序中执行`return 0`,相当于`exit(0)`,直接退出程序。
  - 如果在子程序之中，那么就有区别了
    - 执行`return 0`，会返回到调用该子程序的位置，而不会退出整个进程。
    - 执行`exit(0)`，会直接退出整个进程。

## p13 intro/daytimetcpsrv.c 时间获取程序（服务端）

-  htonl()作用和htons()相同，也是将主机字节顺序转化为网络字节顺序，不同的是，htonl()处理的是32位，htons处理的是16位
  - `htonl()`可以用于处理ip地址，因为一个ip地址的最大位数刚好为32位（0.0.0.0 ~ 255.255.255.255，256=2^8,4*8=32)。
  - `htons()`一般用于处理端口。

## p724 lib/error.c 错误处理函数 

- 分析下自定义函数`void err_sys(const char *fmt,...)`

  ```c++
  void
  err_sys(const char *fmt, ...)
  {
      va_list        ap;
  
      va_start(ap, fmt);
      err_doit(1, LOG_ERR, fmt, ap);
      va_end(ap);
      exit(1);
  }
  ```

  - `va_list`宏定义了一个指针类型，这个指针类型指向参数列表中的参数。
  - `void va_start(va_list ap, last_arg)`
    - 修改了用va_list申明的指针，比如ap，使这个指针指向了不定长参数列表省略号前的参数。
  - `type va_arg(va_list, type)`
    - 获取参数列表的下一个参数，并以type的类型返回。
  - `void va_end(va_list ap)`
    -  参数列表访问完以后，参数列表指针与其他指针一样，必须收回，否则出现野指针。
    - 一般va_start 和va_end配套使用。

- 分析下自定义函数`static void err_doit(int errnoflag, int level, const char *fmt, va_list ap)`

  ```c++
  static void
  err_doit(int errnoflag, int level, const char *fmt, va_list ap)
  {
      int     errno_save, n;
      char    buf[MAXLINE + 1];
  
      errno_save = errno;        /* value caller might want printed */
  #ifdef    HAVE_VSNPRINTF
      vsnprintf(buf, MAXLINE, fmt, ap);    /* safe */
  #else
      vsprintf(buf, fmt, ap);                    /* not safe */
  #endif
      n = strlen(buf);
      if (errnoflag)
          snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
      strcat(buf, "\n");
  
      if (daemon_proc) {
          syslog(level, buf);
      } else {
          fflush(stdout);        /* in case stdout and stderr are the same */
          fputs(buf, stderr);
          fflush(stderr);
      }
      return;
  }
  ```

  - `errno_save = errno`，这里要注意的是全局变量`errno`，为了防止和正常的返回值混淆，系统调用一般并不直接返回错误码，而是将错误码（是一个整数值，不同的值代表不同的含义）存入一个名为 `errno`的全局变量中，`errno` 不同数值所代表的错误消息定义在` <errno.h>` 文件中。
  - ` int vsprintf(char *str, const char *format, va_list arg)` ：使用参数列表发送格式化输出到字符串。
    - `int vsnprintf(char *str, size_t size,const char *format, va_list arg)`：相比而言多了一个限制格式化输出的最大字节数，这样的好处是加了一道保险，防止了内存溢出。
  - `errnoflag`：这个变量若为真，则表明此时有错误发生，且错误码被保存在了`errno`里。
  - `int snprintf(char *str, size_t size, const char *format, ...)`：设将可变参数(...)按照 format 格式化成字符串，并将字符串复制到 str 中，size 为要写入的字符的最大数目，超过 size 会被截断。
  -  `int fputs(const char *str, FILE *stream)`：把字符串写入到指定的流 stream 中，但不包括空字符。
  - `int fflush(FILE *stream)`： 刷新流 stream 的输出缓冲区。

## 内存对齐

- 元素是按照定义顺序一个一个放到内存中去的，但并不是紧密排列的。从结构体存储的首地址开始，每个元素放置到内存中时，它都会认为内存是按照自己的大小（通常它为4或8）来划分的，因此元素放置的位置一定会在自己宽度的整数倍上开始，这就是所谓的[内存对齐](https://cloud.tencent.com/developer/article/1727794)。

## 共用体(union)

- [共用体](http://c.biancheng.net/view/2035.html)的所有成员占用同一段内存，修改一个成员会影响其余所有成员。

- ```c
      union {
          short s;
          char c[sizeof(short)];
      } un;
  ```

  - 像上面这种结构体就可以实现一种操作：通过对字符数组`c`的访问，来观察变量`s`的某个内存地址的具体值。



# 第3章 套接字编程简介

## P68 函数`inet_ntoa()`是一个不可重入的函数

- 在C语言中，内存共分为五个区：
  - 代码区：存放源程序的二进制代码，其余四个区都存储进程运行过程中需要的存储的变量。
  - 栈区：由**编译器**自动分配释放，存放函数的参数值，局部变量的值等。
    - 局部变量在函数内部定义的变量，存储在栈数据区当中。**局部变量只作用于函数内部，当函数运行结束，就将其分配出去的内存再拿回。**
    - const 修饰的局部变量也是放在栈区里的，而不是放在常量存储区。
  - 堆区
    - 由用户自己管理的内存区域，如果分配了之后不能很好的进行回收，会造成**内存泄漏。**
  - 全局/静态存储区
    - 存储全局变量（即在程序中直接定义而非在任何函数体中定义的变量）和静态变量（通过关键字`static`定义的变量，又分为全局静态变量和局部静态变量）。
    - 静态变量和全局变量都会在内存中存储直至程序运行结束。值得注意的是局部静态变量仍然只在其作用域内有效，而由于其值一直存储在内存中，所以下次在其作用域内使用时，会在“原来值”的基础上使用，因此，使用局部静态变量的函数即为**不可重入的函数**。
  - 常量存储区
    - 存储字符串常量。
    - 全局的const变量。
    - 常量存储区中的内容可读不可写。
- 函数`inet_ntoa()`中使用了静态变量，因此是**不可重入的**，因为在程序中如果多次调用这个函数，那么会影响到之前函数的结果。

# 第四章 基本TCP套接字编程

## P83 extern关键字

- 用于仅对一个变量进行声明，而不进行定义。

