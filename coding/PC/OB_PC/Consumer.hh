#pragma once

namespace wd
{
    class TaskQueue; // 前向声明

    class Consumer
    {
    public:
        void consume(TaskQueue &);
    };

} // end of namespace wd