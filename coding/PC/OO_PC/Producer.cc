#include "Producer.hh"
#include "TaskQueue.hh"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

    void Producer::run()
    {
        int cnt = 20;
        int number = 1;
        while (cnt--)
        {
            _taskQue.push(number);
            cout << ">> 生产者线程" << pthread_self() << " 生产了一个number = " << number << endl;
            ++number;
            // ::sleep(0.5);
        }
    }

} // end of namespace wd