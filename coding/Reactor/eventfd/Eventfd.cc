#include "Eventfd.hh"
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <sys/eventfd.h>
#include <sys/poll.h>
#include <unistd.h>
using std::cout;
using std::endl;

namespace wd
{

Eventfd::Eventfd(EventfdCallback &&cb)
    : _eventfd(createEventfd()),
      _cb(std::move(cb)),
      _isStarted(false) {}

Eventfd::~Eventfd()
{
    if (_isStarted) {
        stop();
    }
}
void Eventfd::start()
{
    _isStarted = true;
    struct pollfd pfds;
    pfds.events = POLLIN;
    pfds.fd = _eventfd;
    while (_isStarted) {
        int readyn = ::poll(&pfds, 1, 10000);
        if (readyn == -1 && errno == EINTR) {
            continue;
        } else if (readyn == -1) {
            perror("poll");
            break;
        } else if (readyn == 0) { // 超时打印
            cout << ">> poll timeout" << endl;
        } else {
            if (pfds.revents & POLLIN) {
                handleRead(); // 处理事件
                if (_cb) {
                    _cb();
                }
            }
        }
    }
}
// 在另一个线程执行
void Eventfd::stop()
{
    _isStarted = false;
}

int Eventfd::createEventfd()
{
    int eventfd = ::eventfd(0, 0);
    if (eventfd == -1) {
        perror("eventfd");
    }
    return eventfd;
}

void Eventfd::handleRead()
{
    uint64_t cnt;
    int ret = ::read(_eventfd, &cnt, sizeof cnt);
    if (ret != sizeof(cnt)) {
        perror("read");
    }
}
// wakeup和hanleRead运行在不同线程
void Eventfd::wakeup()
{
    uint64_t cnt = 1;
    int ret = ::write(_eventfd, &cnt, sizeof cnt);
    if (ret != sizeof(cnt)) {
        perror("write");
    }
}

} //end of namespace wd