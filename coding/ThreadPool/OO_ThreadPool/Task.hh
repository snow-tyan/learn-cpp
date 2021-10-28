#pragma once

namespace wd
{

    class Task
    {
    public:
        virtual ~Task(){}
        virtual void process() = 0;
    };

} // namespace wd
