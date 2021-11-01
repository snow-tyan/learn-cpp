#pragma once

#include "InetAddress.hh"
#include "NonCopyable.hh"
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

    string recv();
    void send(const string &);
    string toString() const; // 打印 本机ip port 对端ip port

private:
    InetAddress _localAddr;
    InetAddress _peerAddr;
    Socket _sock;
};

} // end of namespace wd