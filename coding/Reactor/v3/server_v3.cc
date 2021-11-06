#include "TCPServer.hh"
#include "ThreadPool.hh"
#include <iostream>
#include <string>
using namespace std;
using namespace wd;

ThreadPool *gThreadpool = nullptr;

#if 1
class MyTask
{
public:
    MyTask(const string &msg, const TCPConnectionPtr &conn)
        : _msg(msg), _conn(conn) {}

    //运行在线程池的某一个子线程中
    void process()
    {
        //decode
        //compute
        //encode
        cout << ">>> " << tls::threadName << " is working..." << endl;
        cout << ">>> MyTask::process()  " << endl;
        string response = _msg; //要返回给客户端的消息
        //由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
        //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
        //将send的函数的执行延迟到IO线程取操作，交给EventLoop中的loop执行。
        // _conn->send(response);
        _conn->sendInLoop(response);
    }

private:
    string _msg;
    TCPConnectionPtr _conn;
};
#endif

//回调函数体现了扩展性
void onConnection(const TCPConnectionPtr &conn)
{
    cout << conn->toString() << " has connected!" << endl;
    conn->sendInLoop("welcome to server.");
}

void onMessage(const TCPConnectionPtr &conn)
{
    cout << "onMessage...." << endl;
    string msg = conn->recv();
    cout << ">> recv msg from client: " << msg << endl;
    //业务逻辑的处理要交给线程池处理
    //decode
    //compute
    //encode

    //::sleep(2);//碰到需要长时间的处理时，响应速度会降下来
    //conn->send(msg);
    MyTask task(msg, conn);

    //拿到线程池之后，就将该任务交给线程池去执行
    gThreadpool->addTask(std::bind(&MyTask::process, task));
}

void onClose(const TCPConnectionPtr &conn)
{
    cout << "onClose...." << endl;
    cout << conn->toString() << " has disconnected!" << endl;
}

int main(void)
{
    ThreadPool threadpool(4, 10);
    threadpool.start();
    gThreadpool = &threadpool;

    TCPServer server("172.25.40.81", 8888);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setCloseCallback(onClose);

    server.start();
    return 0;
}