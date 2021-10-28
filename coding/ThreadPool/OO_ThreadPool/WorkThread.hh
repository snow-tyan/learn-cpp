#include "Thread.hh"

namespace wd
{
    class ThreadPool; // 没有设计实现，用前向声明
    class WorkThread
        : public Thread
    {
    public:
        WorkThread(ThreadPool &);
        ~WorkThread();

    private:
        void run() override;

    private:
        ThreadPool &_threadpool;
    };
} // end of namespace wd