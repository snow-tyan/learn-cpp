#include "Thread.hh"
#include <iostream>
using namespace std;

namespace wd
{
    Thread::Thread(ThreadCallback &&cb)
        : _pthid(0), _isRunning(false), _cb(std::move(cb)) {}

    void *Thread::threadFunc(void *arg)
    {
        Thread *pthread = static_cast<Thread *>(arg);
        if (pthread)
            pthread->_cb();
        return nullptr;
    }

    void Thread::start()
    {
        pthread_create(&_pthid, nullptr, threadFunc, this);
        _isRunning = true;
    }

    void Thread::join()
    {
        if (_isRunning)
        {
            pthread_join(_pthid, nullptr);
            _isRunning = false;
        }
    }

    Thread::~Thread()
    {
        if (_isRunning)
        {
            pthread_detach(_pthid); // detach掉的线程，其资源自动交还给系统
        }
    }

} // end of namespace wd