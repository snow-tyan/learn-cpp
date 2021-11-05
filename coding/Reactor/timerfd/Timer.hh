#pragma once

#include <functional>
using std::function;

namespace wd
{

using TimerCallback = function<void()>;

class Timer
{
public:
    Timer(int, int, TimerCallback &&);
    ~Timer();
    void start();
    void stop();

private:
    int createTimerFd();
    int getTimer();
    void setTimerFd(int,int);
    void handleRead();

private:
    int _timerfd;
    TimerCallback _cb;
    int _initialTime;
    int _periodicTime;
    bool _isStarted;
};

} // end of namespace wd