#include "Eventfd.hh"
#include "EventfdThread.hh"
#include "Thread.hh"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std;
using namespace wd;

class Task
{
public:
    void process()
    {
        ::srand(::clock());
        int num = ::rand() % 100;
        cout << tls::threadName << " produce num=" << num << endl;
    }
};

void test()
{
    EventfdThread eventfd(std::bind(&Task::process, Task()), "ChildThread");
    eventfd.start();

    int cnt = 15;
    while (cnt--) {
        cout << "main thread notify child thread doing something..." << endl;
        eventfd.wakeup();
        ::sleep(1);
    }
    eventfd.stop();
}
int main()
{
    test();
    return 0;
}