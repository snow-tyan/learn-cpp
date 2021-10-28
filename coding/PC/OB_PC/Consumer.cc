#include "Consumer.hh"
#include "TaskQueue.hh"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

    void Consumer::consume(TaskQueue &taskQue)
    {
        int cnt = 20;
        while (cnt--)
        {
            ::sleep(1);
            int number = taskQue.pop();            
            cout << ">> 消费者线程" << pthread_self() << " 消费了一个number = " << number << endl;
        }
    }

} // end of namespace wd