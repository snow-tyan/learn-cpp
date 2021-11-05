#pragma once

#include "TaskQueue.hh"
#include "Thread.hh"
#include <functional>
#include <memory>
#include <vector>
using std::unique_ptr;
using std::vector;

namespace wd
{
using Task = std::function<void()>; //Task是个函数对象
class ThreadPool
{
public:
    ThreadPool(size_t, size_t);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(Task &&);

private:
    void threadFunc(); // 线程执行体
    Task getTask();

private:
    size_t _threadNum;
    vector<unique_ptr<Thread>> _threads;
    size_t _queSize;
    TaskQueue _taskQue;
    bool _isExit;
};

} // end of namespace wd