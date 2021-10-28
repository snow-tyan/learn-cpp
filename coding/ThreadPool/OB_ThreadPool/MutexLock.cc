#include "MutexLock.hh"
#include <stdio.h>
#include <errno.h>

namespace wd
{

    MutexLock::MutexLock()
    {
        if (pthread_mutex_init(&_mutex, nullptr))
            perror("pthread_mutex_init");
    }
    MutexLock::~MutexLock()
    {
        if (pthread_mutex_destroy(&_mutex))
            perror("pthread_mutex_destory");
    }
    void MutexLock::lock()
    {
        if (pthread_mutex_lock(&_mutex))
            perror("pthread_mutex_lock");
    }
    void MutexLock::unlock()
    {
        if (pthread_mutex_unlock(&_mutex))
            perror("pthread_mutex_unlock");
    }
    pthread_mutex_t *MutexLock::getMutexPtr()
    {
        return &_mutex;
    }

    MutexLockGuard::MutexLockGuard(MutexLock &mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }
    MutexLockGuard::~MutexLockGuard()
    {
        _mutex.unlock();
    }

} // end of namespace wd