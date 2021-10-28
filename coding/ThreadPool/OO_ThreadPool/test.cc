#include "ThreadPool.hh"
#include "Task.hh"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <memory>
using namespace std;
using namespace wd;

class MyTask
    : public Task
{
public:
    void process() override
    {
        // ::srand(::clock());
        // int number = ::rand() % 100;
        cout << "我是子线程" << pthread_self() << endl;
        // ::sleep(1);
    }
};

int main()
{
    ThreadPool threadpool(4, 10); // 4个线程
    threadpool.start();

    unique_ptr<Task> task(new MyTask());
    int cnt = 20;
    while (cnt--)
    {
        threadpool.addTask(task.get());
        cout << "Main线程" << endl;
    }
    threadpool.stop();

    return 0;
}