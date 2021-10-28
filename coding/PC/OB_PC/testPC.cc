#include "TaskQueue.hh"
#include "Thread.hh"
#include "Producer.hh"
#include "Consumer.hh"

#include <iostream>
#include <memory>
#include <functional>
using namespace std;
using namespace wd;

int main()
{
    TaskQueue que(10);
    unique_ptr<Thread> producer(new Thread(
        // std::bind(&Producer::produce, Producer(), que))); // 一堆报错
        // TaskQue被拷贝了，内部Mutex等也被拷贝了，需要使用ref传引用
        std::bind(&Producer::produce, Producer(), std::ref(que)))); 

    unique_ptr<Thread> consumer(new Thread(
        std::bind(&Consumer::consume, Consumer(), std::ref(que))));

    producer->start();
    consumer->start();
    producer->join();
    consumer->join();
    return 0;
}