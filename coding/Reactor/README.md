## 常见并发服务器方案

### Reactor模式
> Reactor模式，又称反应器模式，基于同步IO复用，**单线程**处理多客户端请求

+ read      接收数据
+ 业务逻辑处理
  + decode    解码（拆包）
  + compute   业务逻辑处理，时间不宜太长
  + encode    编码（打包）
+ write     发送数据

### Reactor + ThreadPool
> 业务逻辑较复杂时，单线程处理太慢，结合线程池  
> Reactor处理数据发送接收-->IO线程  
> ThreadPool进行业务逻辑处理-->计算线程
> 涉及计算线程和IO线程的通信  
> 如果大并发，还不能满足需求，MultiReactor+ThreadPool

### Proactor模式
> 主动器模式，基于异步IO  
> boost.asio实现的异步，Linux上实质是用epoll模拟，windows上真正的异步

---

## Reactor封装
1. InetAddress类 -- 负责把`ip`和`port`写入`struct sockaddr`
2. Socket类      -- 负责托管套接字描述符 （创建+销毁）
3. SocketIO类    -- 负责发送和接收数据 `read+write`
4. Acceptor类    -- `bind+listen+accept`
5. TCPConnect类  -- 外层接口
6. EventLoop类   -- `epoll`事件循环，注册、监听、删除
7. TCPServer类


### v1
![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/Reactor-v1.png)
只有前五类，调用方式
```c++
Acceptor acceptor(ip, port); // socket
acceptor.ready(); // bind + listen
TCPConnection tcp(acceptor.accept()); // accept产生peerfd
tcp.send(string);
cout << tcp.recv() << endl;
```


### v2
![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/Reactor-v2.png)
加入`EventLoop`类（即`epoll`）  

网络编程时涉及三个半事件：（事件处理器）
  + 新连接到来时  回调函数`onConnection()`;
  + 消息到达时    回调函数`onMessage()`;  // 业务逻辑处理`encode compute decode`
  + 连接关闭时    回调函数`onClose()`;
  + 消息发送完成时

这些事件每个连接(`peerfd`)都有，因此需要扩充到`TCPconnection`  

```c++
Acceptor acceptor(ip, port);
acceptor.ready();
EventLoop event(acceptor);
event.setConnectionCallback(onConnection);
event.setMessageCallback(onMessage);
event.setCloseCallback(onClose);
event.loop();
```


### v2.1
把`Acceptor`和`EventLoop`组合到一起，封装成`TCPServer`
```c++
TCPServer server(ip, port);
server.setConnectionCallback(..);
//setMessage
//setClose
server.start();
```


### v3
加入线程池（基于对象的线程池）  
把业务逻辑处理交给子线程去完成 -- 子线程完成任务后**消息发送**交给IO线程  
                          -- 涉及计算线程和IO线程之间的通信  `eventfd`  


### Reactor模型序列图
![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/Reactor-timeseq.png)
