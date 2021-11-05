#include "TimerThread.hh"

namespace wd
{

TimerThread::TimerThread(int initialTime, int periodicTime, TimerCallback &&cb)
    : _timer(initialTime, periodicTime, std::move(cb)),
      _thread(std::bind(&Timer::start, &_timer)) {}

void TimerThread::start()
{
    _thread.start(); // _timer.start()绑定到thread里执行
}
void TimerThread::stop()
{
    _timer.stop();
    _thread.join();
}

} // end of namespace wd