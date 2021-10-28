#include "TaskQueue.hh"

namespace wd
{

    TaskQueue::TaskQueue(size_t size)
        : _queSize(size), _mutex(),
          _notFull(_mutex), _notEmpty(_mutex) {}

    // Producer Thread
    void TaskQueue::push(ElemType elem)
    {
        MutexLockGuard autolock(_mutex);
        while (full())
        { // 使用while防止 虚假唤醒（异常唤醒）
            _notFull.wait();
        }
        _que.push(elem);    // 放数据
        _notEmpty.notify(); // 通知消费者线程
    }

    // Consumer Thread
    ElemType TaskQueue::pop()
    {
        MutexLockGuard autolock(_mutex);
        while (empty())
        { // 使用while防止 虚假唤醒（异常唤醒）
            _notEmpty.wait();
        }
        ElemType elem = _que.front();
        _que.pop();        // 取数据
        _notFull.notify(); // 通知生产者线程
        return elem;
    }
    bool TaskQueue::full() const
    {
        return _que.size() == _queSize;
    }
    bool TaskQueue::empty() const
    {
        return _que.empty();
    }

}