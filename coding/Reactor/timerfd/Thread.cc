#include "Thread.hh"
#include <iostream>
using namespace std;

namespace wd
{

namespace current_thread
{
//只能修饰POD类型（内置+C方式结构体）
__thread const char *threadName = "AnonymousThread";
} // end of namespace current_thread

struct ThreadData {
    ThreadData(string name, ThreadCallback &&cb)
        : _name(name), _cb(cb) {}
    void runInThread()
    {
        // 任务执行前
        current_thread::threadName =
            _name != string() ? _name.c_str() : "AnonymousThread";
        // 执行任务
        if (_cb)
            _cb();
        // 任务执行后
    }
    string _name;
    ThreadCallback _cb;
};

void *Thread::threadFunc(void *arg)
{
    ThreadData *pData = static_cast<ThreadData *>(arg);
    pData->runInThread();
    return nullptr;
}

void Thread::start()
{
    ThreadData *pData = new ThreadData(_name, std::move(_cb));
    pthread_create(&_pthid, nullptr, threadFunc, pData);
    _isRunning = true;
}

void Thread::join()
{
    if (_isRunning) {
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}

Thread::~Thread()
{
    if (_isRunning) {
        pthread_detach(_pthid); // detach掉的线程，其资源自动交还给系统
    }
}

} // end of namespace wd