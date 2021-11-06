#pragma once
#include "NonCopyable.hh"
#include <functional>
#include <pthread.h>
#include <string>
using std::string;

namespace wd
{

namespace tls
{
extern __thread const char *threadName; // 全局变量在不同的地方重复定义，加extern表声明

} // end of namespace tls

using ThreadCallback = std::function<void()>;

class Thread
    : public Noncopyable
{
public:
    // cb已经是个右值，无需再std::move了，不过加了也没事
    Thread(ThreadCallback &&cb, string name = string())
        : _pthid(0), _isRunning(false), _cb(cb), _name(name){};
    ~Thread();
    void start();
    void join();
    static void *threadFunc(void *); // 干掉this指针

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;
    string _name;
};

} // end of namespace wd