#include "Thread.hh"
#include "Timer.hh"
#include "TimerThread.hh"
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
        cout << "produce num=" << num << endl;
    }
};

void test0()
{
    Timer timer(1, 11, std::bind(&Task::process, Task()));
    timer.start();
    timer.stop(); // 在本线程根本走不到这一步
}
void test1()
{
    Timer timer(1, 11, std::bind(&Task::process, Task()));
    Thread thread(std::bind(&Timer::start, &timer), "Timer Thread");
    thread.start();
    ::sleep(15);
    timer.stop();
    thread.join();
}
void test2()
{
    TimerThread timer(1, 11, std::bind(&Task::process, Task()));
    timer.start();
    ::sleep(15);
    timer.stop();
}
int main()
{
    // test0();
    // test1();
    test2();
    return 0;
}