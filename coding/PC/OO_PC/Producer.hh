#pragma once

#include "Thread.hh"

namespace wd
{
    class TaskQueue; // 前向声明

    class Producer
        : public Thread
    {
    public:
        Producer(TaskQueue &taskQue) : _taskQue(taskQue) {}

    private:
        void run() override;

    private:
        TaskQueue &_taskQue;
    };

} // end of namespace wd
