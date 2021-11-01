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
1. InetAddress类 -- 
2. Socket类
3. SocketIO类
4. Acceptor类
5. TCPConnect类
6. EventLoop类
7. TCPServer类
