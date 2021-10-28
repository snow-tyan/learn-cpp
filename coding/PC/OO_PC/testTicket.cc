/*
封装互斥锁和条件变量
希望二者生命周期独立
pthread_mutex_t mutex;
pthread_mutex_lock(&mutex);
在执行
pthread_cond_wait(pthread_cont_t *, pthread_mutex_t *);
之前，要先加锁，之后当前线程A阻塞，在阻塞之前要先解锁
当另外的线程B调用
pthread_cond_signal(pthread_cond_t *)
之后，唤醒线程A，线程A被唤醒时，要先加锁

单向关联关系：条件变量必须知道互斥锁的存在，而互斥锁不知道条件变量
*/
#include "MutexLock.hh"
#include "Condition.hh"
#include <unistd.h>
#include <iostream>
using namespace std;
using namespace wd;

//全局变量，消除全局变量，封成类，用传参的方式
// int ticket = 20;
// MutexLock mutex;
// Condition cond(mutex);
// bool flag = true;

class Ticket
{
public:
    Ticket(int ticket, MutexLock &mutex, Condition &cond, bool flag)
        : _ticket(ticket), _mutex(mutex), _cond(cond), _flag(flag) {}

public:
    int _ticket;
    MutexLock &_mutex;
    Condition &_cond;
    bool _flag;
};

void *window1(void *arg)
{
    Ticket *pT = (Ticket *)arg;
    int &ticket = pT->_ticket;
    MutexLock &mutex = pT->_mutex;
    Condition &cond = pT->_cond;
    bool &flag = pT->_flag;
    while (ticket)
    {
        // mutex.lock();
        MutexLockGuard autolock(mutex); //RAII 用对象的生命周期来管理锁
        while (!flag) //使用while防止异常唤醒或虚假唤醒
            cond.wait();
        if (!ticket)
            break;
        --ticket;
        cout << "win1 sale, left ticket:" << ticket << endl;

        if (flag)
            flag = false;
        ::sleep(1);
        cond.notify();
        // mutex.unlock();
    }
    return nullptr;
}
void *window2(void *arg)
{
    Ticket *pT = (Ticket *)arg;
    int &ticket = pT->_ticket;
    MutexLock &mutex = pT->_mutex;
    Condition &cond = pT->_cond;
    bool &flag = pT->_flag;
    while (ticket)
    {
        // mutex.lock();
        MutexLockGuard autolock(mutex);
        while (flag)
            cond.wait();
        if (!ticket)
            break;
        --ticket;
        cout << "win2 sale, left ticket:" << ticket << endl;
        if (!flag)
            flag = true;
        ::sleep(1);
        cond.notify();
        // mutex.unlock();
    }
    return nullptr;
}

int main()
{
    MutexLock mutex;
    Condition cond(mutex);
    Ticket t(20, mutex, cond, true);

    pthread_t pthid1, pthid2;
    pthread_create(&pthid1, nullptr, window1, &t);
    pthread_create(&pthid2, nullptr, window2, &t);
    pthread_join(pthid1, nullptr);
    pthread_join(pthid2, nullptr);
    return 0;
}