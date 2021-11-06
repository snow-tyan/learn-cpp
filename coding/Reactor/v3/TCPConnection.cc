#include "TCPConnection.hh"
#include "EventLoop.hh"
#include <sstream>
using std::ostringstream;

namespace wd
{

TCPConnection::TCPConnection(int fd, EventLoop *event)
    : _sock(Socket(fd)),
      _sockio(SocketIO(fd)),
      _localAddr(getLocalAddr(fd)),
      _peerAddr(getPeerAddr(fd)),
      _isShutdownWrite(false),
      _event(event) {}

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
    // _sockio.readn(buff, sizeof buff);
    return string(buff);
}

void TCPConnection::sendInLoop(const string &msg)
{
    if (_event) {
        _event->runInLoop(std::bind(&TCPConnection::send, this, msg));
    }
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

void TCPConnection::setConnectionCallback(const TCPCallback &cb)
{
    _onConnection = cb;
}
void TCPConnection::setMessageCallback(const TCPCallback &cb)
{
    _onMessage = cb;
}
void TCPConnection::setCloseCallback(const TCPCallback &cb)
{
    _onClose = cb;
}
void TCPConnection::handleConnectionCallback()
{
    if (_onConnection) {
        _onConnection(shared_from_this());
    }
}
void TCPConnection::handleMessageCallback()
{
    if (_onMessage) {
        _onMessage(shared_from_this());
    }
}
void TCPConnection::handleCloseCallback()
{
    if (_onClose) {
        _onClose(shared_from_this());
    }
}

} // end of namespace wd