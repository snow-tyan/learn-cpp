#include "SocketIO.hh"

#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace wd
{

SocketIO::SocketIO(int fd) : _fd(fd) {}

int SocketIO::readn(char *buff, int len)
{
    int left = len;
    char *p = buff;
    while (left > 0) {
        int ret = ::read(_fd, p, left);
        if (-1 == ret && errno == EINTR) { // 系统调用被中断，重试
            continue;
        } else if (-1 == ret) { // 出错
            perror("read");
            return len - left;
        } else if (0 == ret) { // 连接断开
            return len - left;
        } else { // 正常读取的情况
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}
int SocketIO::readLine(char *buff, int maxLen) // 一行最大值maxLen
{
    /**
     * recv 第四个参数设置为0时，和read功能相同。
     * 都是从内核态拷贝数据到用户态，并且把内核态相应数据移走。
     * 
     * 想读取一行数据，读到\n后面添加\0，\n后面的数据是并不是脏数据，是下一行的
     * 因此我们想从内核态拷贝数据但不移走内核态相应数据。recv四参数MSG_PEEK
     * **/
    int left = maxLen - 1; // 最后一位留给'\0'
    char *p = buff;
    int total = 0;
    while (left > 0) {
        int ret = ::recv(_fd, p, left, MSG_PEEK);
        if (-1 == ret) {
            perror("recv msg_peek");
        }
        // 查找'\n'
        for (int i = 0; i < ret; ++i) {
            if (p[i] == '\n') { // 找到了，多读一位，留给'\0'
                readn(p, i + 1);
                total += (i + 1);
                p += (i + 1);
                *p = '\0';
                return total;
            }
        }
        readn(p, ret);
        left -= ret;
        p += left;
        total += ret;
    }
    *p = '\0';
    return total;
}
int SocketIO::writen(const char *buff, int len)
{
    int left = len;
    const char *p = buff;
    while (left > 0) {
        int ret = ::write(_fd, p, left);
        if (-1 == ret && errno == EINTR) {
            continue;
        } else if (-1 == ret) {
            perror("write");
            return len - left;
        } else {
            left -= ret;
            p += ret;
        }
    }
    return len - left;
}

} // end of namespace wd