#pragma once

#include "NonCopyable.hh"
#include <pthread.h>

namespace wd
{

    class MutexLock
        : public NonCopyable
    {
    public:
        MutexLock();
        ~MutexLock();
        void lock();
        void unlock();
        pthread_mutex_t* getMutexPtr();

    private:
        pthread_mutex_t _mutex;
    };

    class MutexLockGuard
    {
    public:
        MutexLockGuard(MutexLock &);
        ~MutexLockGuard();

    private:
        MutexLock &_mutex;
    };

} // end of namespace wd