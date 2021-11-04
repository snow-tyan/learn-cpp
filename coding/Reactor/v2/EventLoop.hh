#pragma once

#include "TCPConnection.hh"
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

private:
    void epollWait();
    void handleNewConnection();
    void handleNewMessage(int); // peerfd
    int epollCreate();
    void addEpollFdRead(int);
    void delEpollFdRead(int);
    bool isConnectClosed(int); // 连接是否断开

private:
    int _epfd;
    Acceptor &_acceptor;
    vector<struct epoll_event> _eventList;
    // {peerfd: tcp*}
    // shared_ptr<TCPConnection>;
    map<int, TCPConnectionPtr> _conns;
    bool _isLooping;

    TCPCallback _onConnection;
    TCPCallback _onMessage;
    TCPCallback _onClose;
};

} // end of namespace wd