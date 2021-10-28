#include "WorkThread.hh"
#include "ThreadPool.hh"
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

    WorkThread::WorkThread(ThreadPool &threadpool)
        : _threadpool(threadpool)
    {
        cout << "WorkThread()" << endl;
    }
    WorkThread::~WorkThread()
    {
        cout << "~WorkThread()" << endl;
    }

    void WorkThread::run()
    {
        _threadpool.threadFunc(); // 调用线程执行体 threadFunc
    }

} // end of namespace wd