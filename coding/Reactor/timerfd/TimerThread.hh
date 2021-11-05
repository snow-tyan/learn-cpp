#pragma once

#include "Thread.hh"
#include "Timer.hh"

namespace wd
{

class TimerThread
{
public:
    TimerThread(int, int, TimerCallback &&);
    void start();
    void stop();

private:
    Timer _timer;
    Thread _thread;
};

} // end of namespace wd