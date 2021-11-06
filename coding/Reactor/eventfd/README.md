## Linux进程间通信-eventfd
> eventfd是Linux 2.6提供的一种系统调用，它可以用来实现事件通知。eventfd包含一个由内核维护的64位无符号整型计数器，创建eventfd时会返回一个文件描述符，进程可以通过对这个文件描述符进行read/write来读取/改变计数器的值，从而实现进程间通信。


### 创建
```c
#include <sys/eventfd.h>
int eventfd(unsigned int initval, int flags);
```
+ initval:计数器初始值
+ flags：EFD_CLOEXEC  EFD_NONBLOCK  EFD_SEMAPHORE  现在的内核版本填0即可

### 读取
用read读取，注意：
+ read会从eventfd对应的64bit计数器读取8字节整型变量
+ buf不能低于8字节，否则出错，errno=EINVAL
+ read返回值按小端字节序
+ 如果eventfd设置flags=EFD_SEMAPHORE，那么read就会返回1并让计数器减一；如果没设置上述flags，read就会直接返回计数器中的数值，read之后计数器置零。
+ 计数器为0时，read会阻塞（eventfd未设置非阻塞）或EAGAIN错误（设置了非阻塞）。

### 写
用write写，注意：
+ 同样的，如果eventfd未设置EFD_SEMAPHORE，会将buf中数据写入计数器，最大8字节，否则EINVAL错误
+ 设置了EFD_SEMAPHORE，会向计数器加法操作。写满阻塞（未设置非阻塞）或返回EAGAIN错误码（设置非阻塞）。


#### select/poll/epoll

### 封装eventfd
![](https://gitee.com/snow-tyan/learn-cpp/raw/master/Figure/eventfd.png)