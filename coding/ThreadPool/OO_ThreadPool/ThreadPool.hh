#pragma once

#include "Thread.hh"
#include "TaskQueue.hh"
#include <vector>
#include <memory>
using std::unique_ptr;
using std::vector;

namespace wd
{
    class Task;

    class ThreadPool
    {
    public:
        ThreadPool(size_t, size_t);
        ~ThreadPool();
        void start();
        void stop();
        void addTask(Task *);

    private:
        friend class WorkThread;
        void threadFunc(); // 线程执行体
        Task *getTask();

    private:
        size_t _threadNum;
        vector<unique_ptr<Thread>> _threads;
        size_t _queSize;
        TaskQueue _taskQue;
        bool _isExit;
    };

} // end of namespace wd