# Kqueue

## 数据结构

- `kevent`结构体

  ```c
  struct kevent {
      // 该事件关联的文件描述符，如socket中的fd句柄
      uintptr_t       ident;
      /* 
       * 可以指定监听类型
       * 如EVFILT_READ=读，EVFILT_WRITE=写，EVFILT_TIMER=定时器事件，EVFILT_SIGNAL=信号，EVFILT_USER=用户自定义事件
       */
      int16_t         filter;     
      /**
       * 操作方式
       * EV_ADD 添加，EV_DELETE 删除，EV_ENABLE 激活，EV_DISABLE 不激活
       */
      uint16_t        flags;
      uint32_t        fflags;         /* 第二种操作方式，NOTE_TRIGGER 立即激活等等 */
      intptr_t        data;           /* int 型的用户数据，socket 里面它是可读写的数据长度 */
      void            *udata;         /* 指针类型的数据，你可以携带任何想携带的附加数据。比如对象 */
  };
  ```

## API

- `kqueue`：用于初始化`kqueue`句柄

- `EV_SET`：用于初始化`kevent`结构的宏

  ```c
  EV_SET(&kev, ident, filter, flags, fflags, data, udata);
  ```

- `kevent`：组合了epoll_ctl及epoll_wait功能，changelist与nchanges为修改列表，eventlist与nevents为返回的事件列表

  ```c
  int kevent(int kq,                      // kqueue的句柄
      const struct kevent *changelist, 		// kevent 的数组，就是一次可以添加多个事件
      int nchanges, 											// changelist 数组长度
      struct kevent *eventlist, 					// 待接收事件的数组，里面是空的，接收系统调用返回的数据
      int nevents, 												// eventlist 数组长度
      const struct timespec *timeout); 		// 是阻塞超时时间，超过这个时间就不阻塞了，直接返回
  ```

  

