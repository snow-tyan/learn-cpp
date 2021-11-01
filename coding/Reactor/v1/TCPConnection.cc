#include "TCPConnection.hh"

#include <sstream>
using std::ostringstream;

namespace wd
{

TCPConnection::TCPConnection(int fd)
    : _sock(Socket(fd)),
      _sockio(SocketIO(fd)),
      _localAddr(getLocalAddr(fd)),
      _peerAddr(getPeerAddr(fd)),
      _isShutdownWrite(false) {}

TCPConnection::~TCPConnection()
{
    if (!_isShutdownWrite) {
        shutdown();
    }
}
void TCPConnection::shutdown()
{
    if (!_isShutdownWrite) {
        _isShutdownWrite = true;
        _sock.shutdownWrite();
    }
}
string TCPConnection::toString() const
{
    ostringstream oss;
    oss << _localAddr.ip() << ":" << _localAddr.port()
        << "-->" << _peerAddr.ip() << ":" << _peerAddr.port();
    return oss.str();
}
string TCPConnection::recv()
{
    char buff[65536] = {0};
    _sockio.readLine(buff, sizeof(buff));
    return string(buff);
}
void TCPConnection::send(const string &msg)
{
    _sockio.writen(msg.c_str(), msg.size());
}

InetAddress TCPConnection::getLocalAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if (-1 == getsockname(_sock.fd(), (struct sockaddr *)&addr, &len)) {
        perror("getsockname");
    }
    return InetAddress(addr);
}
InetAddress TCPConnection::getPeerAddr(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    if (-1 == getpeername(_sock.fd(), (struct sockaddr *)&addr, &len)) {
        perror("getpeername");
    }
    return InetAddress(addr);
}

} // end of namespace wd