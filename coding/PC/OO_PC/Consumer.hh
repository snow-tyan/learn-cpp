#pragma once

#include "Thread.hh"

namespace wd
{
    class TaskQueue; // 前向声明

    class Consumer
        : public Thread
    {
    public:
        Consumer(TaskQueue &taskQue) : _taskQue(taskQue) {}

    private:
        void run() override;

    private:
        TaskQueue &_taskQue;
    };

} // end of namespace wd