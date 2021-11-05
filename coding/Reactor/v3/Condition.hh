#pragma once

#include "MutexLock.hh"

namespace wd
{

class Condition
    : public NonCopyable
{
public:
    Condition(MutexLock &);
    ~Condition();
    void wait();
    void notify();
    void notifyAll();

private:
    MutexLock &_mutex;
    pthread_cond_t _cond;
};

} // end of namespace wd