#pragma once
#include "Condition.hh"
#include <functional>
#include <queue>
using std::queue;

namespace wd
{
using ElemType = std::function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t);
    void push(ElemType);
    ElemType pop();
    bool full() const;
    bool empty() const;
    void wakeup();

private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
    bool _runFlag; // 保证队列为空依然能退出
};

} // end of namespace wd