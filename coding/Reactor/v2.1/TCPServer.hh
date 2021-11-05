#pragma once

#include "Acceptor.hh"
#include "EventLoop.hh"
#include "TCPConnection.hh"

namespace wd
{

class TCPServer
{
public:
    TCPServer(const string &, unsigned short);
    void start();

    // 回调函数，交给EventLoop，再交给TCPConnection执行
    void setConnectionCallback(TCPCallback &&);
    void setMessageCallback(TCPCallback &&);
    void setCloseCallback(TCPCallback &&);

private:
    Acceptor _acceptor;
    EventLoop _event;
};

} // end of namespace wd