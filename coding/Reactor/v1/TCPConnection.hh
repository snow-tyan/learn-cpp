#pragma once

#include "InetAddress.hh"
#include "NonCopyable.hh"
#include "Socket.hh"
#include "SocketIO.hh"
#include <string>
using std::string;

namespace wd
{

class TCPConnection
    : NonCopyable
{
public:
    TCPConnection(int); // fd
    ~TCPConnection();

    string recv();
    void send(const string &);
    string toString() const; // 打印 本机ip port -> 对端ip port
    void shutdown();

private:
    // 为了初始化_localAddr和_peerAddr
    InetAddress getLocalAddr(int);
    InetAddress getPeerAddr(int);

private:
    Socket _sock;
    SocketIO _sockio;
    // 为了打印 ip port
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite; // 主动断开连接
};

} // end of namespace wd