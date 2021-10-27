#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "Thread.hh"
#include <iostream>
#include <memory>
using namespace std;
using namespace wd;

class Task
{
public:
    void run()
    {
        ::srand(::clock());
        while (1)
        {
            int number = ::rand() % 100;
            cout << "Task::run, number = " << number << endl;
            ::sleep(1);
        }
    }
};

void run2()
{
    ::srand(::clock());
    while (1)
    {
        int number = ::rand() % 100;
        cout << "run2, number = " << number << endl;
        ::sleep(1);
    }
}

int main(void)
{
    // auto f = std::bind(&Task::run, Task());
    // auto f = std::mem_fn(&Task::run);
    unique_ptr<Thread> thread(new Thread(std::bind(&Task::run, Task())));
    // unique_ptr<Thread> thread(new Thread(run2)); // 注册回调函数
    
    thread->start();
    thread->join(); // 主线程等待子线程
    return 0;
}