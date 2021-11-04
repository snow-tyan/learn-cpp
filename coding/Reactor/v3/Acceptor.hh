#pragma once

#include "InetAddress.hh"
#include "Socket.hh"

namespace wd
{

class Acceptor
{
public:
    // server: socket - bind - listen - while(1){accept - recv - send} - close
    // client: socket -------------------------- connect - send - recv - close
    explicit Acceptor(unsigned short);        // port
    Acceptor(const string &, unsigned short); // ip port

    void ready(); // bind + listen
    int accept(); // peerfd
    int sockfd(); // socketfd

private:
    void bind();
    void listen();
    void setReuseAddr(bool);
    void setReusePort(bool);

private:
    InetAddress _addr;
    Socket _sock;
};

} // end of namespace wd