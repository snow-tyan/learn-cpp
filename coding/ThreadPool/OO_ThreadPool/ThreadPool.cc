#include "ThreadPool.hh"
#include "WorkThread.hh"
#include <unistd.h>

namespace wd
{

    ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
        : _threadNum(threadNum), _queSize(queSize),
          _taskQue(queSize), _isExit(false)
    {
        _threads.reserve(threadNum); // 只是开辟空间，但是size仍然=0
    }
    ThreadPool::~ThreadPool()
    {
        if (_isExit)
            stop();
    }

    void ThreadPool::start()
    {
        // 创建线程对象，存入vector
        for (size_t i = 0; i != _threadNum; ++i)
        {
            unique_ptr<Thread> thread(new WorkThread(*this));
            _threads.push_back(std::move(thread)); // unique_ptr不能复制，只能用移动语义
        }
        // 让每个子线程跑起来（真正创建了子线程(pthread_create())）
        for (auto &thread : _threads)
        {
            thread->start(); // 线程启动
        }
    }

    // 让子线程执行任务
    void ThreadPool::threadFunc()
    {
        while (!_isExit) // 线程池不退出，就一直循环
        {
            Task *task = getTask();
            if (task)
                task->process(); // 执行任务
        }
    }

    void ThreadPool::stop()
    {
        if (!_isExit)
        {
            // 任务执行完，再退出
            while (!_taskQue.empty())
                usleep(1);
            // 当任务执行速度过快，再将_isExit设置为True之前，
            // 每个子线程都已经取任务，此时发现taskQue之中没有任务
            // 所有子线程都会阻塞在notEmpty的条件变量之上，从而无法退出
            _isExit = true;
            _taskQue.wakeup(); // _notEmpty.notifyAll();让所有的notEmpty条件变量成立
            //回收每个子线程
            for (auto &thread : _threads)
                thread->join();
        }
    }

    void ThreadPool::addTask(Task *task) // 生产者
    {
        _taskQue.push(task);
    }

    Task *ThreadPool::getTask() // 消费者
    {
        return _taskQue.pop();
    }

} // end of namespace wd