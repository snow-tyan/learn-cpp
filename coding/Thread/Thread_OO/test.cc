#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "Thread.hh"
#include <iostream>
#include <memory>
using namespace std;
using namespace wd;

class MyThread
    : public Thread
{
public:
    void run() override
    {
        ::srand(::clock());
        while (1)
        {
            int number = ::rand() % 100;
            cout << "number = " << number << endl;
            ::sleep(1);
        }
    }
};

int main(void)
{
    unique_ptr<Thread> thread(new MyThread());
    thread->start();
    thread->join(); // 主线程等待子线程
    return 0;
}