#include "EventfdThread.hh"

namespace wd
{

EventfdThread::EventfdThread(EventfdCallback &&cb, string name)
    : _eventfd(std::move(cb)),
      _thread(std::bind(&Eventfd::start, &_eventfd), name) {}

void EventfdThread::start()
{
    _thread.start(); // _eventfd.start()绑定到thread里执行
}
void EventfdThread::stop()
{
    _eventfd.stop();
    _thread.join();
}
void EventfdThread::wakeup()
{
    _eventfd.wakeup();
}

} // end of namespace wd