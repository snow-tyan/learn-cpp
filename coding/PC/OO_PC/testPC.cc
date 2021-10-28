#include "TaskQueue.hh"
#include "Producer.hh"
#include "Consumer.hh"

#include <iostream>
#include <memory>

using namespace std;
using namespace wd;

int main()
{
    TaskQueue que(10);
    unique_ptr<Thread> producer(new Producer(que));
    unique_ptr<Thread> consumer(new Consumer(que));

    producer->start();
    consumer->start();
    producer->join();
    consumer->join();
    return 0;
}