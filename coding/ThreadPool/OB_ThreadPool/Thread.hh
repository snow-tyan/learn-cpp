#pragma once
#include <pthread.h>
#include <functional>

namespace wd
{
    using ThreadCallback = std::function<void()>;
    class Thread
    {
    public:
        Thread(ThreadCallback &&);
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