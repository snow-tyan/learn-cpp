#include "TCPServer.hh"

namespace wd
{

TCPServer::TCPServer(const string &ip, unsigned short port)
    : _acceptor(ip, port), _event(_acceptor) {}

void TCPServer::start()
{
    _acceptor.ready();
    _event.loop();
}

void TCPServer::setConnectionCallback(TCPCallback &&cb)
{
    _event.setConnectionCallback(std::move(cb));
}
void TCPServer::setMessageCallback(TCPCallback &&cb)
{
    _event.setMessageCallback(std::move(cb));
}
void TCPServer::setCloseCallback(TCPCallback &&cb)
{
    _event.setCloseCallback(std::move(cb));
}

} // end of namespace wd