#include "Socket.hh"

#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace wd
{

Socket::Socket()
{
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == _fd) {
        perror("socket");
    }
}
Socket::Socket(int fd) : _fd(fd) {}
Socket::~Socket() { ::close(_fd); }

int Socket::fd() { return _fd; }

// 主动断开连接 关闭写端
void Socket::shutdownWrite()
{
    // SHUT_RDWR SHUT_RD SHUT_WR
    // shutdown 与 close
    // close同时关闭读写端并释放套接字，而shutdown只关闭。
    ::shutdown(_fd, SHUT_WR);
}

} // namespace wd