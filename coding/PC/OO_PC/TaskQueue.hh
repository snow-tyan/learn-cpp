#pragma once

#include "Condition.hh"
#include <queue>
using std::queue;

namespace wd
{
    using ElemType = int;
    class TaskQueue
    {
    public:
        TaskQueue(size_t);
        void push(ElemType elem);
        ElemType pop();
        bool full() const;
        bool empty() const;

    private:
        size_t _queSize;
        queue<int> _que;
        MutexLock _mutex;
        Condition _notFull;
        Condition _notEmpty;
    };

} // end of namespace wd