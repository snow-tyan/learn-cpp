#include "InetAddress.hh"

namespace wd
{

InetAddress::InetAddress(unsigned short port)
{
    ::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);       // host to net short
    _addr.sin_addr.s_addr = INADDR_ANY; // inet_addr("0.0.0.0") 本机地址，一个服务器可能有多个网卡
}
InetAddress::InetAddress(const string &ip, unsigned short port)
{
    ::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port); // host to net short
    _addr.sin_addr.s_addr = ::inet_addr(ip.c_str());
}
InetAddress::InetAddress(const struct sockaddr_in addr)
    : _addr(addr) {}

string InetAddress::ip() const
{
    return string(::inet_ntoa(_addr.sin_addr));
}
unsigned short InetAddress::port() const
{
    return ::ntohs(_addr.sin_port); // net to host
}
struct sockaddr_in *InetAddress::getAddrPtr()
{
    return &_addr;
}

} // end of namespace wd