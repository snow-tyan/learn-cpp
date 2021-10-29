#pragma once
#include <pthread.h>
#include <functional>
#include <string>
using std::string;

namespace wd
{
    // TLS
    namespace tls
    {
        extern __thread const char *threadName;
    } // end of namespace tls

    using ThreadCallback = std::function<void()>;
    class Thread
    {
    public:
        Thread(ThreadCallback &&, string = string());
        ~Thread();
        void start();
        void join();
        static void *threadFunc(void *); // 干掉this指针

    private:
        pthread_t _pthid;
        bool _isRunning;
        ThreadCallback _cb;
        string _name; // 给线程命一个名字
    };

} // end of namespace wd