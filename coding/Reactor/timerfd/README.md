## Linux定时器
> 基于文件描述符的定时器，可以被select/epoll监听到

### 相关接口
```c
#include <sys/timerfd.h>
int timerfd_create(int clockid, int flags);
int timerfd_settime(int fd, int flags,
                    const struct itimerspec *new_value,
                    struct itimerspec *old_value);
int timerfd_gettime(int fd, struct itimerspec *curr_value);
```

### create
> 返回定时器描述符timerfd
+ clockid 指定时间类型
  + CLOCK_REALTIME   
  + CLOCK_MONOTONIC
+ flags  内核版本xxx以后填0即可

### settime
```c
struct timespec{
    time_t tv_sec; // 秒
    long   tv_nsec;// 纳秒
}

struct itimerspec{
    struct timespec it_interval; // 超时周期
    struct timespec it_value;    // 第一次超时时间
}
```
+ fd: timerfd
+ new_value 新的超时时间
+ old_value 上次设置的超时时间 传出参数 不想知道设为null
+ flags: 1 绝对时间 0 相对时间


### gettime
> 用于获得定时器距离下次超时还剩下的时间

+ curr_value 传出参数


### 封装timerfd
![]()