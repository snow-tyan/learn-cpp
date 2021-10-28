#pragma once

namespace wd
{
    class TaskQueue; // 前向声明

    class Producer
    {
    public:
        void produce(TaskQueue &);
    };

} // end of namespace wd
