#pragma once
#include <pthread.h>

namespace wd
{

    class Thread
    {
    public:
        Thread() : _pthid(0), _isRunning(false){};
        virtual ~Thread();
        void start();
        void join();
        static void *threadFunc(void *); // 干掉this指针

    private:
        virtual void run() = 0;

    private:
        pthread_t _pthid;
        bool _isRunning;
    };

} // end of namespace wd