#pragma once

namespace wd
{

class Socket
{
public:
    // RAII托管套接字，构造创建套接字，析构释放套接字
    Socket();
    explicit Socket(int);
    ~Socket();

    int fd();
    void shutdownWrite(); // 主动断开连接 关闭写端

private:
    int _fd;
};


}// end of namespace wd