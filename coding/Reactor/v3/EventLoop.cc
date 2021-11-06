#include "EventLoop.hh"
#include "Acceptor.hh"
#include <assert.h>
#include <iostream>
#include <sys/eventfd.h>
#include <unistd.h>
using std::cout;
using std::endl;

namespace wd
{

EventLoop::EventLoop(Acceptor &acceptor)
    : _epfd(epollCreate()),
      _acceptor(acceptor),
      _eventList(1024), // vector传int构造
      _isLooping(false),
      _eventfd(createEventfd())
{
    // 注册sockfd
    addEpollFdRead(_acceptor.sockfd());
    // 注册eventfd
    addEpollFdRead(_eventfd);
}
void EventLoop::loop()
{
    _isLooping = true;
    while (_isLooping) {
        epollWait();
    }
}
void EventLoop::unloop()
{ // 当前线程是退不出来了，在另一个线程里执行unloop
    if (_isLooping) {
        _isLooping = false;
    }
}
void EventLoop::runInLoop(Functor &&cb)
{
    // 语句块，只针对放回调函数加锁，放完解锁
    {
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}
void EventLoop::epollWait()
{
    // 通过对迭代器解引用再取地址拿到数组首地址，vector的迭代器底层就是指针
    int readyn;
    do { // 中断信号导致的readyn==-1不管它继续执行
        readyn = epoll_wait(_epfd, &*_eventList.begin(),
                            _eventList.size(), 10000); // 10s超时
    } while (readyn == -1 && errno == EINTR);
    if (readyn == -1) { // 非中断信号导致的reayn==-1
        perror("epoll_wait");
        return;
    } else if (readyn == 0) {
        cout << ">> epoll wait time out <<" << endl;
    } else {
        if (readyn == _eventList.size()) {
            _eventList.resize(2 * readyn); // 扩容
        }
        for (int i = 0; i < readyn; ++i) {
            int fd = _eventList[i].data.fd;
            if (fd == _acceptor.sockfd()) { // sockfd
                if (_eventList[i].events & EPOLLIN) {
                    handleNewConnection();
                }
            } else if (fd == _eventfd) {
                if (_eventList[i].events & EPOLLIN) {
                    handleRead();
                    cout << "doPendingFunctors()..." << endl;
                    doPendingFunctors();
                }
            } else { // peerfd
                if (_eventList[i].events & EPOLLIN) {
                    handleNewMessage(fd);
                }
            }
        }
    }
}
void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollFdRead(peerfd); // 注册peerfd到监听实例
    TCPConnectionPtr conn(new TCPConnection(peerfd, this));
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);

    _conns[peerfd] = conn; // 存入map中
    conn->handleConnectionCallback();
}
void EventLoop::handleNewMessage(int fd)
{
    bool isClosed = isConnectClosed(fd);
    auto it = _conns.find(fd);
    assert(it != _conns.end());

    if (!isClosed) {
        it->second->handleMessageCallback();
    } else {
        delEpollFdRead(fd);
        it->second->handleCloseCallback();
        _conns.erase(fd);
    }
}
bool EventLoop::isConnectClosed(int fd)
{
    int ret;
    do {
        char buff[1024] = {0};
        ret = recv(fd, buff, sizeof buff, MSG_PEEK);
    } while (ret == -1 && errno == EINTR);
    return ret == 0; // ret==0 连接断开
}

int EventLoop::epollCreate()
{
    int epfd = epoll_create1(0);
    if (-1 == epfd) {
        perror("epoll_create1");
    }
    return epfd;
}
void EventLoop::addEpollFdRead(int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN; //监听读事件
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
    if (-1 == ret) {
        perror("epoll_ctl add fd");
    }
}
void EventLoop::delEpollFdRead(int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &event);
    if (-1 == ret) {
        perror("epoll_ctl del fd");
    }
}
void EventLoop::setConnectionCallback(TCPCallback &&cb)
{
    _onConnection = std::move(cb);
}
void EventLoop::setMessageCallback(TCPCallback &&cb)
{
    _onMessage = std::move(cb);
}
void EventLoop::setCloseCallback(TCPCallback &&cb)
{
    _onClose = std::move(cb);
}

int EventLoop::createEventfd()
{
    int efd = ::eventfd(0, 0);
    if (efd == -1) {
        perror("eventfd");
    }
    return efd;
}

void EventLoop::handleRead()
{
    uint64_t cnt;
    int ret = ::read(_eventfd, &cnt, sizeof cnt);
    if (ret != sizeof cnt) {
        perror("handleRead");
    }
}

void EventLoop::wakeup()
{
    uint64_t cnt;
    int ret = ::write(_eventfd, &cnt, sizeof cnt);
    if (ret != sizeof cnt) {
        perror("wakeup write");
    }
}

void EventLoop::doPendingFunctors()
{
    // 每次执行回调函数都要加锁，效率太低了
    // 尽可能减少加锁次数

    // 先把_pendingFunctors和temp做交换，再遍历temp
    // 去执行回调函数
    vector<Functor> temp;
    {
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.swap(temp); // O(1) 只交换底层的三个指针
    }
    for (auto &functor : temp) {
        functor();
    }
}

} // end of namespace wd