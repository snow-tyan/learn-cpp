#include "Timer.hh"
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/timerfd.h>
#include <unistd.h>
using std::cout;
using std::endl;

namespace wd
{

Timer::Timer(int initialTime, int periodicTime, TimerCallback &&cb)
    : _timerfd(createTimerFd()),
      _initialTime(initialTime),
      _periodicTime(periodicTime),
      _cb(std::move(cb)),
      _isStarted(false) {}

Timer::~Timer()
{
    if (_isStarted) {
        stop();
    }
}
void Timer::start()
{
    _isStarted = true;
    setTimerFd(_initialTime, _periodicTime);
    struct pollfd pfds;
    pfds.events = POLLIN;
    pfds.fd = _timerfd;
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
void Timer::stop()
{
    setTimerFd(0, 0); // 停止定时器
    _isStarted = false;
}

int Timer::createTimerFd()
{
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    if (timerfd == -1) {
        perror("timer_create");
    }
    return timerfd;
}

void Timer::setTimerFd(int initialTime, int periodicTime)
{
    struct itimerspec time;
    time.it_value.tv_sec = initialTime;
    time.it_value.tv_nsec = 0;
    time.it_interval.tv_sec = periodicTime;
    time.it_interval.tv_nsec = 0;
    int ret = timerfd_settime(_timerfd, 0, &time, nullptr);
    if (-1 == ret) {
        perror("timerfd_settime");
    }
}

void Timer::handleRead()
{
    unsigned long cnt; // 超市次数
    int ret = ::read(_timerfd, &cnt, sizeof cnt);
    if (ret != sizeof(cnt)) {
        perror("read");
    }
}

} //end of namespace wd