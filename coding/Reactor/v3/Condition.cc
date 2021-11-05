#include "Condition.hh"
#include <errno.h>
#include <stdio.h>

namespace wd
{

Condition::Condition(MutexLock &mutex)
    : _mutex(mutex)
{
    if (pthread_cond_init(&_cond, nullptr))
        perror("pthread_cond_init");
}
Condition::~Condition()
{
    if (pthread_cond_destroy(&_cond))
        perror("pthread_cond_destory");
}
void Condition::wait()
{
    if (pthread_cond_wait(&_cond, _mutex.getMutexPtr()))
        perror("pthread_cond_wait");
}
void Condition::notify()
{
    if (pthread_cond_signal(&_cond))
        perror("pthread_cond_signal");
}
void Condition::notifyAll()
{
    if (pthread_cond_broadcast(&_cond)) {
        perror("pthread_cond_broadcast");
    }
}

} // end of namespace wd