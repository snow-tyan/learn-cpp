### tcp chat


#### v1
+ 完成管道通信所具备功能，有退出机制。一端断开，另一端自然断开
  ```c
  fd_set rdset;
  FD_ZERO(&rdset);
  FD_SET(stdin);
  FD_SET(peerfd);
  while(1){
    ret = select(maxfds, &rdset, ...);
    if (FD_ISSET(stdin))
    if (FD_ISSET(peerfd))
  }
  ```

+ bug：服务器断开后再启动，有bind:Address already in use


#### v2
+ 需求：服务器一般不断开，所以客户端断开时，服务器不退出。
  + 设计思路：server端 select 既监控peerfd，也监控socketfd
  + 用另一个fdset needMoniterSetl; 记录需要监控的描述符，它不直接传给select
  + 而是先memcpy给rdset，让select监控rdset，每次socketfd就绪时把新的peerfd加入needMonitorSet，client端关闭时就把相应的peerfd从needMonitorSet中移除

  ```c
  fd_set rdset, needMoniterSet;
  FD_ZERO(&needMoniterSet);
  FD_SET(stdin);
  FD_SET(peerfd);
  while(1){
    memcpy(&rdset, &needMoniterSet, sizeof(fd_set));
    ret = select(maxfds, &rdset, ...);
    if (FD_ISSET(socketfd)) { accept ...}
    if (FD_ISSET(stdin))
    if (FD_ISSET(peerfd))
  }
  ```

+ 解决bug：setsockopt，设置地址、端口可重用


#### v3
+ epoll 替换 select
  ```c
  epoll_create1(0);
  struct epoll_event event;// 注册、删除事件
  epoll_ctl(EPOLL_CTL_ADD);
  struct epoll_event evs[MAX];
  while (1){
    readyn = epoll_wait(evs,...);
    for loop:
        监听socketfd
        监听stdin
        监听peerfd
  }
  ```

#### v3.1
+ 非阻塞模式，可以一次把缓冲区内数据读空
  + 非阻塞（传文件描述符），缓冲区无内容就返回-1，错误码11