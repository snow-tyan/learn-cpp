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
#include <pthread.h>
#include <iostream>
using namespace std;

class MutexLock
{
public:
    MutexLock()
    {
        cout << "MutexLock()" << endl;
        if (pthread_mutex_init(&_mutex, nullptr))
        {
            perror("pthread_mutex_init");
        }
    }
    ~MutexLock()
    {
        cout << "~MutexLock()" << endl;
        if (pthread_mutex_destroy(&_mutex))
        {
            perror("pthread_mutex_destory");
        }
    }
    void lock()
    {
        if (pthread_mutex_lock(&_mutex))
        {
            perror("pthread_mutex_lock");
        }
    }
    void unlock()
    {
        if (pthread_mutex_unlock(&_mutex))
        {
            perror("pthread_mutex_unlock");
        }
    }
    pthread_mutex_t *getMutexPtr()
    {
        return &_mutex;
    }

    MutexLock(const MutexLock &) = delete;
    MutexLock &operator=(const MutexLock &) = delete;

private:
    pthread_mutex_t _mutex;
};

class Condition
{
public:
    Condition(MutexLock &mutex)
        : _mutex(mutex)
    {
        cout << "Condition(MutexLock &)" << endl;
        if (pthread_cond_init(&_cond, nullptr))
        {
            perror("pthread_cond_init");
        }
    }

    ~Condition()
    {
        cout << "~Condition()" << endl;
        if (pthread_cond_destroy(&_cond))
        {
            perror("pthread_cond_destory");
        }
    }
    void wait()
    {
        if (pthread_cond_wait(&_cond, _mutex.getMutexPtr()))
        {
            perror("pthread_cond_wait");
        }
    }
    void notify()
    {
        if (pthread_cond_signal(&_cond))
        {
            perror("pthread_cond_singal");
        }
    }
    void notifyAll()
    {
        if (pthread_cond_broadcast(&_cond))
        {
            perror("pthread_cond_broadcast");
        }
    }

    Condition(const Condition &) = delete;
    Condition &operator=(const Condition &) = delete;

private:
    pthread_cond_t _cond;
    MutexLock &_mutex;
};

class MutexLockGuard
{
public:
    MutexLockGuard(MutexLock &mutex)
        : _mutex(mutex)
    {
        cout << "MutexLockGuard(MutexLock &)" << endl;
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        cout << "~MutexLockGuard()" << endl;
        _mutex.unlock();
    }

private:
    MutexLock &_mutex;
};

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