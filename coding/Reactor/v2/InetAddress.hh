#pragma once

#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
using std::string;

namespace wd
{

class InetAddress
{
public:
    explicit InetAddress(unsigned short);        // port
    InetAddress(const string &, unsigned short); // ip port
    InetAddress(const struct sockaddr_in);
    string ip() const;
    unsigned short port() const;
    struct sockaddr_in *getAddrPtr();

private:
    struct sockaddr_in _addr;
};

} // end of namespace wd