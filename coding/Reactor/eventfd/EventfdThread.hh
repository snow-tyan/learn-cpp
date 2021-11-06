#pragma once

#include "Eventfd.hh"
#include "Thread.hh"

namespace wd
{

class EventfdThread
{
public:
    EventfdThread(EventfdCallback &&, string name = string());
    void start();
    void stop();
    void wakeup();

private:
    Eventfd _eventfd;
    Thread _thread;
    string _name; // thread name
};

} // end of namespace wd