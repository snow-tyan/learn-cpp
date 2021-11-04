#pragma once

namespace wd
{

class SocketIO
{
public:
    explicit SocketIO(int);        // fd
    int readn(char *, int);        // buf, len
    int readLine(char *, int);     // buf, maxLen 一行最大长度
    int writen(const char *, int); // buf, len

private:
    int _fd;
};

} // end of namespace wd