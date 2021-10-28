#pragma once
#include "NonCopyable.hh"
#include <pthread.h>
#include <functional>

namespace wd
{

    class Thread
        : public Noncopyable
    {
    public:
        using ThreadCallback = std::function<void()>;

        Thread(ThreadCallback &&cb)
            : _pthid(0), _isRunning(false), _cb(std::move(cb)){};
        ~Thread();
        void start();
        void join();
        static void *threadFunc(void *); // 干掉this指针

    private:
        pthread_t _pthid;
        bool _isRunning;
        ThreadCallback _cb;
    };

} // end of namespace wd