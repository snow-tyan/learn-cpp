#include "TaskQueue.hh"

namespace wd
{
    TaskQueue::TaskQueue(size_t size)
        : _queSize(size), _mutex(),
          _notFull(_mutex), _notEmpty(_mutex), _runFlag(true) {}

    // 生产者
    void TaskQueue::push(ElemType elem)
    {
        // 别忘了给临界资源加锁
        MutexLockGuard autolock(_mutex); // 局部对象，该函数执行完销毁
        while (full())
        {
            _notFull.wait();
        }
        _que.push(elem);
        _notEmpty.notify();
    }
    // 消费者
    ElemType TaskQueue::pop()
    {
        MutexLockGuard autolock(_mutex);
        while (_runFlag && empty())
        {
            _notEmpty.wait();
        }
        if (_runFlag)
        {
            ElemType elem = _que.front();
            _que.pop();
            _notFull.notify();
            return elem;
        }
        else
        {
            return nullptr;
        }
    }

    void TaskQueue::wakeup()
    {
        _runFlag = false;
        _notEmpty.notifyAll();
    }

    bool TaskQueue::full() const
    {
        return _que.size() == _queSize;
    }
    bool TaskQueue::empty() const
    {
        return _que.size() == 0; // _que.empty();
    }

} // end of namespace wd