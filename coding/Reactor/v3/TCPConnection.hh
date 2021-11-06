#pragma once

#include "InetAddress.hh"
#include "NonCopyable.hh"
#include "Socket.hh"
#include "SocketIO.hh"
#include <functional>
#include <memory>
#include <string>
using std::enable_shared_from_this;
using std::function;
using std::shared_ptr;
using std::string;

namespace wd
{
class EventLoop;
class TCPConnection;
using TCPConnectionPtr = shared_ptr<TCPConnection>;
using TCPCallback = function<void(const TCPConnectionPtr &)>;

class TCPConnection
    : NonCopyable,
      public enable_shared_from_this<TCPConnection>
{
public:
    TCPConnection(int, EventLoop *); // fd, EventLoop（IO线程）
    ~TCPConnection();

    string recv();
    void sendInLoop(const string &); // 交给IO线程_eventloop
    string toString() const;         // 打印 本机ip port -> 对端ip port
    void shutdown();

    // 回调函数 这里用的是const左值引用，因为这里的cb是EventLoop转交过来的
    // 如果使用右值引用，EventLoop会把cb转交给第一个TCPConnection对象
    // 剩下的cb都会变成空的
    void setConnectionCallback(const TCPCallback &cb);
    void setMessageCallback(const TCPCallback &cb);
    void setCloseCallback(const TCPCallback &cb);
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    // 为了初始化_localAddr和_peerAddr
    InetAddress getLocalAddr(int);
    InetAddress getPeerAddr(int);

    void send(const string &); // 本计算线程内不做发送，交给IO线程去做

private:
    Socket _sock;
    SocketIO _sockio;
    // 为了打印 ip port
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite; // 主动断开连接
    // 回调函数，调用时需要传参 (const shared_ptr<TCPConnection>)
    // 用shared_from_this();
    TCPCallback _onConnection;
    TCPCallback _onMessage;
    TCPCallback _onClose;
    EventLoop *_event;
};

} // end of namespace wd