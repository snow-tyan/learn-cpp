#include "Acceptor.hh"

namespace wd
{

Acceptor::Acceptor(unsigned short port)
    : _addr(InetAddress(port)), _listensock() {}
Acceptor::Acceptor(const string &ip, unsigned short port)
    : _addr(InetAddress(ip, port)), _listensock() {}

void Acceptor::ready()
{
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}

void Acceptor::bind()
{
    // int bind(sockfd, struct sockaddr *, socklen_t);
    int ret = ::bind(_listensock.fd(),
                     (struct sockaddr *)_addr.getAddrPtr(),
                     sizeof(struct sockaddr));
    if (-1 == ret) {
        perror("bind");
    }
}
void Acceptor::listen()
{
    int ret = ::listen(_listensock.fd(), 10);
    if (-1 == ret) {
        perror("listen");
    }
}

int Acceptor::accept()
{
    int peerfd = ::accept(_listensock.fd(), nullptr, nullptr);
    if (-1 == peerfd) {
        perror("accept");
    }
    return peerfd;
}

void Acceptor::setReuseAddr(bool on)
{
    int one = on;
    if (-1 == setsockopt(_listensock.fd(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) {
        perror("setsockopt reuse addr");
    }
}
void Acceptor::setReusePort(bool on)
{
    int one = on;
    if (-1 == setsockopt(_listensock.fd(), SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one))) {
        perror("setsockopt reuse port");
    }
}

} // end of namespace wd