#pragma once

#include "MutexLock.hh"
#include "TCPConnection.hh"
#include <functional>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <vector>
using std::map;
using std::shared_ptr;
using std::vector;

namespace wd
{
class Acceptor;
using Functor = function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor &); // peerfd,sockfd
    void loop();
    void unloop();
    // 回调函数，转交给TCPConnection去做
    void setConnectionCallback(TCPCallback &&cb);
    void setMessageCallback(TCPCallback &&cb);
    void setCloseCallback(TCPCallback &&cb);
    void wakeup();
    void runInLoop(Functor &&);
    void doPendingFunctors(); // 执行_pendingFuntors所有回调函数

private:
    void epollWait();
    void handleNewConnection();
    void handleNewMessage(int); // peerfd
    int epollCreate();
    void addEpollFdRead(int);
    void delEpollFdRead(int);
    bool isConnectClosed(int); // 连接是否断开
    int createEventfd();
    void handleRead();

private:
    int _epfd;
    int _eventfd;
    Acceptor &_acceptor;
    vector<struct epoll_event> _eventList;
    // {peerfd: tcp*}
    // shared_ptr<TCPConnection>;
    map<int, TCPConnectionPtr> _conns;
    bool _isLooping;

    TCPCallback _onConnection;
    TCPCallback _onMessage;
    TCPCallback _onClose;
    // 数组存储，因为可能多个连接，又是临界资源，需互斥锁
    vector<Functor> _pendingFunctors; // 等待被执行的回调函数
    MutexLock _mutex;
};

} // end of namespace wd