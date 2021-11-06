#pragma once

#include <functional>
using std::function;

namespace wd
{

using EventfdCallback = function<void()>;

class Eventfd
{
public:
    Eventfd(EventfdCallback &&);
    ~Eventfd();
    void start();
    void stop();
    void wakeup();

private:
    int createEventfd();
    int getEventfd();
    void handleRead();

private:
    int _eventfd;
    EventfdCallback _cb;
    bool _isStarted;
};

} // end of namespace wd