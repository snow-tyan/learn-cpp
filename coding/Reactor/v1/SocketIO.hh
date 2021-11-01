#pragma once

#include "Socket.hh"

namespace wd
{

class SocketIO
{
public:
    SocketIO(int);                 // fd
    int readn(char *, int);        // buf, len
    int readLine(char *, int);     // buf, maxLen
    int writen(const char *, int); // buf, len

private:
    Socket _sock;
};

} // end of namespace wd