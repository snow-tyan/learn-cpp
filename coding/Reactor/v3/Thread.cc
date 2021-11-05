#include "Thread.hh"
#include <iostream>
using namespace std;

namespace wd
{
// TLS
namespace tls
{
__thread const char *threadName = "AnonymousThread";
} // end of namespace tls

struct ThreadData {
    ThreadData(string name, ThreadCallback &&cb)
        : _name(name), _cb(cb) {}

    void runInThread()
    {
        tls::threadName =
            (_name == string()) ? "AnonymousThread" : _name.c_str();
        if (_cb) {
            _cb(); // 执行回调函数
        }
    }

    string _name;
    ThreadCallback _cb;
};

Thread::Thread(ThreadCallback &&cb, string name)
    : _pthid(0), _isRunning(false), _cb(std::move(cb)), _name(name) {}

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